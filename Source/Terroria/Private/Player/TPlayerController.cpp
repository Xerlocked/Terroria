// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/Player/TPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "NiagaraFunctionLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/TPlayerCharacter.h"
#include "Interface/Highlight.h"

ATPlayerController::ATPlayerController()
{
	ZoomDelta = 50.f;
}

void ATPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
}

void ATPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	TraceCursor();
}

void ATPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : MappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			// Mouse Click
			EnhancedInputComponent->BindAction(DestClickAction, ETriggerEvent::Started, this, &ATPlayerController::DoClickStart);

			// Mouse Wheel Scroll
			EnhancedInputComponent->BindAction(WheelAction, ETriggerEvent::Triggered, this, &ATPlayerController::DoWheel);

			// Press Spacebar
			EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ATPlayerController::DoDash);
		}
	}
}

void ATPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedCharacter = Cast<ATPlayerCharacter>(InPawn);
}

void ATPlayerController::DoClickStart()
{
	FHitResult Hit;
	GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, Hit);

	if (!Hit.bBlockingHit)
	{
		return;
	}

	// Align character direction
	FVector Direction = GetPawn()->GetActorLocation() - Hit.Location;
	GetPawn()->AddMovementInput(Direction.GetSafeNormal());

	// Move to target position
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Hit.Location);

	// Spawn click particle
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FXCursor, Hit.Location);
}

void ATPlayerController::DoWheel(const FInputActionValue& Value)
{
	FVector WheelDelta = Value.Get<FVector>();
	if (IsValid(PossessedCharacter.Get()))
	{
		PossessedCharacter->UpdateCameraZoom(WheelDelta.X * ZoomDelta);
	}
}

void ATPlayerController::DoDash()
{
	// Dash
}

void ATPlayerController::TraceCursor()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (!Hit.bBlockingHit)
	{
		return;
	}
	
	LastActor = CurrentActor;
	CurrentActor = Hit.GetActor();

	if (Hit.GetActor() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.GetActor()->GetName());
	}

	
	if (LastActor == nullptr)
	{
		if (CurrentActor != nullptr)
		{
			// Scenario B.
			CurrentActor->ActiveHighlightActor();
		}
	}
	else
	{
		if (CurrentActor == nullptr)
		{
			// Scenario C.
			LastActor->DeactiveHighlightActor();
		}
		else
		{
			if (LastActor != CurrentActor)
			{
				// Scenario D.
				LastActor->DeactiveHighlightActor();
				CurrentActor->ActiveHighlightActor();
			}
		}
	}
}
