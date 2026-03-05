// Copyright 2025 @xerlock. All Rights Reserved.


#include "Public/Player/TPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "TGameplayTags.h"
#include "AbilitySystem/Component/GameplayInputQueueSystem.h"
#include "Character/TPlayerCharacter.h"
#include "Components/SplineComponent.h"
#include "DialogueSystem/DialogueManagerSubsystem.h"
#include "Input/TGameplayInputComponent.h"
#include "Interface/Highlight.h"
#include "Player/TShopComponent.h"
#include "UI/HUD/THUD.h"

ATPlayerController::ATPlayerController()
{
	ZoomDelta = 50.f;
	RouteSpline = CreateDefaultSubobject<USplineComponent>("RouteSpline");
	InputQueueSystemComponent = CreateDefaultSubobject<UGameplayInputQueueSystem>(TEXT("InputQueueSystemComponent"));
	ShopComponent = CreateDefaultSubobject<UTShopComponent>(TEXT("ShopComponent"));
}

void ATPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : MappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}

	if (InputQueueSystemComponent)
	{
		InputQueueSystemComponent->OnInputConsumed.AddDynamic(this, &ATPlayerController::OnInputConsumed);
	}


	DialogueManager = GetWorld()->GetSubsystem<UDialogueManagerSubsystem>();

	if (!DialogueManager)
	{
		UE_LOG(LogTemp, Error, TEXT("DialogueManager is not available."));
		return;
	}

	DialogueManager->OnDialogueStarted.AddDynamic(
		this, &ATPlayerController::OnDialogueStarted);

	DialogueManager->OnDialogueEnded.AddDynamic(
		this, &ATPlayerController::OnDialogueEnded);

	SetShowMouseCursor(true);
}

void ATPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bIsInDialogue)
	{
		return;
	}

	TraceCursor();
	MovePlayerToDestination();
}

void ATPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	if (IsLocalController())
	{
		PossessedCharacter = Cast<ATPlayerCharacter>(GetPawn());
	}
}

void ATPlayerController::OnInputConsumed(FGameplayTag InputTag)
{
	// bMovingToDestination = false;
	// StopMovement();
}

void ATPlayerController::TraceCursor()
{
	if (bIsPointerOverUI)
	{
		// UI 위에 올렸을 때 기존 몬스터 하이라이트 끄기
		if (CurrentActor)
		{
			CurrentActor->DeactiveHighlightActor();
			CurrentActor = nullptr;
		}

		if (LastActor)
		{
			LastActor->DeactiveHighlightActor();
			LastActor = nullptr;
		}

		SetPlayerCursor(ETerroriaCursor::Normal);
		return;
	}

	GetHitResultUnderCursor(ECC_Visibility, false, CursorTraceHit);

	if (!CursorTraceHit.bBlockingHit)
	{
		SetPlayerCursor(ETerroriaCursor::Normal);
		return;
	}

	LastActor = CurrentActor;
	CurrentActor = CursorTraceHit.GetActor();

	if (LastActor != CurrentActor)
	{
		OnTargeting.Broadcast(CursorTraceHit.GetActor());
		if (LastActor)
		{
			LastActor->DeactiveHighlightActor();
			SetPlayerCursor(ETerroriaCursor::Normal);
		}

		if (CurrentActor)
		{
			CurrentActor->ActiveHighlightActor();
			SetPlayerCursor(ETerroriaCursor::Attack);
		}
	}
}

void ATPlayerController::MovePlayerToDestination()
{
	if (!bMovingToDestination)
	{
		return;
	}

	if (GetTASC())
	{
		FGameplayTag RootTag = FTGameplayTags::Get().State_Cast_Rooted;
		if (GetTASC()->HasMatchingGameplayTag(RootTag))
		{
			bMovingToDestination = false;
			return;
		}
	}

	if (PossessedCharacter)
	{
		const FVector LocationOnSpline = RouteSpline->FindLocationClosestToWorldLocation(
			PossessedCharacter->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector WorldDirection = RouteSpline->FindDirectionClosestToWorldLocation(
			LocationOnSpline, ESplineCoordinateSpace::World);

		PossessedCharacter->AddMovementInput(WorldDirection);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= StopMovementRadius)
		{
			bMovingToDestination = false;
		}
	}
}

void ATPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UTGameplayInputComponent* GameplayInputComponent = Cast<UTGameplayInputComponent>(InputComponent))
	{
		GameplayInputComponent->BindAbilityActions(InputContext, this, &ThisClass::PressedAbilityAction,
		                                           &ThisClass::ReleasedAbilityAction, &ThisClass::HeldAbilityAction);
	}
}

void ATPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedCharacter = Cast<ATPlayerCharacter>(InPawn);
}

void ATPlayerController::DoWheel(const FInputActionValue& Value)
{
	FVector WheelDelta = Value.Get<FVector>();
	if (IsValid(PossessedCharacter.Get()))
	{
		PossessedCharacter->UpdateCameraZoom(WheelDelta.X * ZoomDelta);
	}
}

void ATPlayerController::PressedAbilityAction(const FInputActionValue& Value, FGameplayTag Tag)
{
	if (Tag.MatchesTagExact(FTGameplayTags::Get().Input_Mouse_LMB))
	{
		GetTASC()->TargetConfirm();
	}

	if (Tag.MatchesTagExact(FTGameplayTags::Get().Input_Keyboard_P))
	{
		if (const ATHUD* HUD = Cast<ATHUD>(GetHUD()))
		{
			HUD->ToggleShopWidget();
		}
		return;
	}

	if (Tag.MatchesTagExact(FTGameplayTags::Get().Input_Keyboard_F))
	{
		if (IsValid(PossessedCharacter.Get()))
		{
			PossessedCharacter->ProcessInteraction();
		}
		return;
	}

	if (Tag.MatchesTagExact(FTGameplayTags::Get().Input_Mouse_RMB))
	{
		MousePressTime = 0.f;
		bIsTargeting = CurrentActor ? true : false;
		bMovingToDestination = false;
		GetTASC()->TargetCancel();
	}
	else
	{
		bool bBuffered = false;
		if (InputQueueSystemComponent && InputQueueSystemComponent->IsInputBufferEnabled())
		{
			bBuffered = InputQueueSystemComponent->AddInputToBuffer(Tag);
		}
		else if (GetTASC() && GetTASC()->HasMatchingGameplayTag(FTGameplayTags::Get().State_Attacking))
		{
			return;
		}

		if (!bBuffered && GetTASC())
		{
			GetTASC()->HeldAbilityInputTag(Tag);
		}
	}
}

void ATPlayerController::ReleasedAbilityAction(const FInputActionValue& Value, const FGameplayTag Tag)
{
	if (!Tag.MatchesTagExact(FTGameplayTags::Get().Input_Mouse_RMB))
	{
		// if (GetTASC())
		// {
		// 	GetTASC()->HeldAbilityInputTag(Tag);
		// }
		return;
	}

	if (MousePressTime <= MinPressedThreshold && PossessedCharacter)
	{
		MousePressTime = 0.f;
		bIsTargeting = false;

		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
			this, PossessedCharacter->GetActorLocation(), CachedDestination))
		{
			RouteSpline->ClearSplinePoints();

			if (NavPath->PathPoints.IsEmpty())
			{
				return;
			}

			for (const FVector& PathPoint : NavPath->PathPoints)
			{
				RouteSpline->AddSplinePoint(PathPoint, ESplineCoordinateSpace::World);
				DrawDebugSphere(GetWorld(), PathPoint, 5.0f, 12, FColor::Blue, false, 3.0f);
			}
			CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
			bMovingToDestination = true;
		}
	}
}

void ATPlayerController::HeldAbilityAction(const FInputActionValue& Value, const FGameplayTag Tag)
{
	if (Tag.MatchesTagExact(FTGameplayTags::Get().Input_Mouse_Wheel))
	{
		DoWheel(Value);
		return;
	}

	if (!Tag.MatchesTagExact(FTGameplayTags::Get().Input_Mouse_RMB))
	{
		// if (GetTASC())
		// {
		// 	GetTASC()->HeldAbilityInputTag(Tag);
		// }
		return;
	}

	if (bIsTargeting)
	{
		// if (GetTASC())
		// {
		// 	GetTASC()->HeldAbilityInputTag(Tag);
		// }
	}
	else
	{
		if (GetTASC())
		{
			FGameplayTag RootTag = FTGameplayTags::Get().State_Cast_Rooted;
			if (GetTASC()->HasMatchingGameplayTag(RootTag))
			{
				return;
			}
		}

		MousePressTime += GetWorld()->GetDeltaSeconds();

		if (CursorTraceHit.bBlockingHit)
		{
			CachedDestination = CursorTraceHit.ImpactPoint;
		}

		if (PossessedCharacter)
		{
			const FVector WorldDirection = (CachedDestination - PossessedCharacter->GetActorLocation()).GetSafeNormal();
			PossessedCharacter->AddMovementInput(WorldDirection);
		}
	}
}

void ATPlayerController::OnDialogueStarted(ACharacter* InPlayer, ACharacter* NPC)
{
	if (InPlayer != GetPawn())
	{
		return;
	}

	SetViewTargetWithBlend(NPC, 0.1f, VTBlend_PreBlended);
	PlayerCameraManager->StartCameraFade(1.0f, 0.f, 2.5f, FLinearColor::Black);

	if (GetPawn())
	{
		HiddenActors.Add(GetPawn());
		GetPawn()->DisableInput(this);
		StopMovement();
		bMovingToDestination = false;
	}

	SetShowMouseCursor(true);
	FInputModeUIOnly InputMode;

	SetInputMode(InputMode);

	bIsInDialogue = true;
}

void ATPlayerController::OnDialogueEnded(ACharacter* InPlayer)
{
	if (InPlayer != GetPawn())
	{
		return;
	}

	SetViewTargetWithBlend(InPlayer, 0.45f, VTBlend_Cubic);

	if (GetPawn())
	{
		HiddenActors.Remove(GetPawn());
		GetPawn()->EnableInput(this);
	}

	SetShowMouseCursor(true);
	FInputModeGameAndUI InputMode;

	SetInputMode(InputMode);

	bIsInDialogue = false;
}

UTAbilitySystemComponent* ATPlayerController::GetTASC()
{
	if (TAbilitySystemComponent == nullptr)
	{
		TAbilitySystemComponent = Cast<UTAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return TAbilitySystemComponent;
}
