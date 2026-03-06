// Copyright 2025 @xerlock. All Rights Reserved.

#include "Public/Character/TPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "TGameplayTags.h"
#include "AbilitySystem/TAbilitySystemComponent.h"
#include "AbilitySystem/Data/TLevelUpDataAsset.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "DialogueSystem/PlayerDialogueComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interface/Interactable.h"
#include "Net/UnrealNetwork.h"
#include "Player/TPlayerController.h"
#include "Player/TPlayerState.h"
#include "QuestSystem/QuestReceiverComponent.h"
#include "UI/HUD/THUD.h"

ATPlayerCharacter::ATPlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	bUseControllerRotationYaw = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->SetRelativeRotation(FRotator(-50.0f, 45.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 1450.f;
	SpringArm->CameraLagSpeed = 3.0f;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->FieldOfView = 60.0f;

	LevelUpNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUpNiagara"));
	LevelUpNiagara->SetupAttachment(RootComponent);
	LevelUpNiagara->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	LevelUpNiagara->SetAutoActivate(false);

	FlashNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FlashNiagara"));
	FlashNiagara->SetupAttachment(RootComponent);
	FlashNiagara->SetAutoActivate(false);

	InteractionCollision = CreateDefaultSubobject<USphereComponent>("InteractionCollision");
	InteractionCollision->SetupAttachment(GetRootComponent());
	InteractionCollision->SetCollisionProfileName(TEXT("TriggerAllDynamic"));

	LocalDialogueComponent = CreateDefaultSubobject<UPlayerDialogueComponent>("LocalDialogueComponent");

	QuestReceiverComponent = CreateDefaultSubobject<UQuestReceiverComponent>("QuestReceiverComponent");

	CharacterClass = ECharacterClass::Magician;
}

void ATPlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPlayerCharacter, CurrentComboIndex);
}

void ATPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	SetupAbilityActorInfo();
	AddCharacterAbilities();

	InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &ATPlayerCharacter::OnInteractionOverlapBegin);
	InteractionCollision->OnComponentEndOverlap.AddDynamic(this, &ATPlayerCharacter::OnInteractionOverlapEnd);
}

void ATPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	SetupAbilityActorInfo();
}

int32 ATPlayerCharacter::GetPlayerLevel_Implementation() const
{
	const ATPlayerState* TPlayerState = GetPlayerState<ATPlayerState>();

	return TPlayerState ? TPlayerState->GetPlayerLevel() : 1;
}

int32 ATPlayerCharacter::GetComboIndex_Implementation()
{
	return CurrentComboIndex;
}

void ATPlayerCharacter::AdvanceCombo_Implementation()
{
	CurrentComboIndex++;
	if (CurrentComboIndex > MaxComboCount)
	{
		CurrentComboIndex = 1;
	}
}

void ATPlayerCharacter::ResetCombo_Implementation()
{
	CurrentComboIndex = 1;
}

FVector ATPlayerCharacter::GetWeaponSocketLocation_Implementation(FName SocketName) const
{
	return GetMesh()->GetSocketLocation(SocketName);
}

void ATPlayerCharacter::LevelUP_Implementation()
{
	MulticastLevelUpParticles();
}

void ATPlayerCharacter::AddToXP_Implementation(int32 NewXP)
{
	ATPlayerState* TPlayerState = GetPlayerState<ATPlayerState>();
	check(TPlayerState);
	TPlayerState->AddToXP(NewXP);
}

void ATPlayerCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	ATPlayerState* TPlayerState = GetPlayerState<ATPlayerState>();
	check(TPlayerState);
	TPlayerState->AddToLevel(InPlayerLevel);
}

void ATPlayerCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	ATPlayerState* TPlayerState = GetPlayerState<ATPlayerState>();
	check(TPlayerState);
	TPlayerState->AddToAttributePoint(InAttributePoints);
}

int32 ATPlayerCharacter::GetXP_Implementation() const
{
	ATPlayerState* TPlayerState = GetPlayerState<ATPlayerState>();
	check(TPlayerState);
	return TPlayerState->GetXP();
}

int32 ATPlayerCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	ATPlayerState* TPlayerState = GetPlayerState<ATPlayerState>();
	check(TPlayerState);
	return TPlayerState->LevelUpInfo->LevelUpsInformation[Level].AttributePointReward;
}

int32 ATPlayerCharacter::FindLevelForXP_Implementation(int32 XP) const
{
	ATPlayerState* TPlayerState = GetPlayerState<ATPlayerState>();
	check(TPlayerState);
	return TPlayerState->LevelUpInfo->FindLevelForXP(XP);
}

int32 ATPlayerCharacter::GetAttributePoint_Implementation() const
{
	ATPlayerState* TPlayerState = GetPlayerState<ATPlayerState>();
	check(TPlayerState);
	return TPlayerState->GetAttributePoint();
}

ATPlayerController* ATPlayerCharacter::GetPlayerController_Implementation() const
{
	return Cast<ATPlayerController>(GetController());
}

int32 ATPlayerCharacter::GetCurrentGold_Implementation() const
{
	const ATPlayerState* TPlayerState = GetPlayerState<ATPlayerState>();
	check(TPlayerState);
	return TPlayerState->GetGold();
}

bool ATPlayerCharacter::SpendGold_Implementation(int32 Amount)
{
	ATPlayerState* TPlayerState = GetPlayerState<ATPlayerState>();
	check(TPlayerState);

	if (TPlayerState->GetGold() < Amount)
	{
		return false;
	}

	TPlayerState->AddToGold(-Amount);
	return true;
}

int32 ATPlayerCharacter::GetItemLevel_Implementation(FGameplayTag EventTag) const
{
	if (!AbilitySystemComponent)
	{
		return 0;
	}

	// @Note: 먼저, EventTag과 매핑된 Attribute의 값을 찾습니다.
	if (const FGameplayAttribute* TargetAttribute = TagToAttributeMap.Find(EventTag))
	{
		bool bFound = false;
		const float CurrentValue = AbilitySystemComponent->GetGameplayAttributeValue(*TargetAttribute, bFound);

		return bFound ? FMath::FloorToInt(CurrentValue) : 0;
	}

	// @Note: 매핑된 Attribute가 없는 경우, 스킬 레벨에서 찾습니다.
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if (Spec.Ability->GetAssetTags().HasTagExact(EventTag))
		{
			return Spec.Level;
		}
	}

	return 0; // 찾지 못한 경우 0을 반환
}

void ATPlayerCharacter::UpgradeAbilityByTag_Implementation(const FGameplayTag& AbilityTag)
{
	if (!HasAuthority() || !AbilitySystemComponent)
	{
		return;
	}

	// 장착된 모든 스킬을 순회하며 일치하는 태그를 찾습니다.
	for (FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		// Spec이 가진 태그 중 구매한 아이템의 EventTag와 일치하는 것이 있다면
		if (Spec.Ability->GetAssetTags().HasTagExact(AbilityTag))
		{
			Spec.Level += 1; // 레벨 1 증가
			AbilitySystemComponent->MarkAbilitySpecDirty(Spec);

			// UI 업데이트를 위한 델리게이트 호출 (이전에 구현해두신 로직)
			if (UTAbilitySystemComponent* TASC = Cast<UTAbilitySystemComponent>(AbilitySystemComponent))
			{
				TASC->AbilityLevelChangedEvent.Broadcast(AbilityTag, Spec.Level);
			}
			break;
		}
	}
}

UQuestReceiverComponent* ATPlayerCharacter::GetQuestReceiverComponent() const
{
	return QuestReceiverComponent;
}

void ATPlayerCharacter::AddExperience(int32 Amount)
{
}

void ATPlayerCharacter::AddCurrency(int32 Amount)
{
}

void ATPlayerCharacter::AddItem(FName ItemID, int32 Quantity)
{
}

void ATPlayerCharacter::AddGameplayTag(FGameplayTag Tag)
{
}

void ATPlayerCharacter::ProcessInteraction()
{
	if (InteractionActor)
	{
		if (InteractionActor->Implements<UInteractable>())
		{
			IInteractable::Execute_Interact(InteractionActor, this);
		}
	}
}

void ATPlayerCharacter::HandleDeath_Implementation()
{
	Super::HandleDeath_Implementation();

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}

	if (ATPlayerController* PlayerController = Cast<ATPlayerController>(GetController()))
	{
		if (PlayerController->IsLocalController())
		{
			PlayerController->FlushPressedKeys();
			PlayerController->SetIgnoreMoveInput(true);
			PlayerController->SetIgnoreLookInput(true);

			DisableInput(PlayerController);
			FInputModeUIOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = true;
		}
	}
}

void ATPlayerCharacter::OnInteractionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != InteractionActor)
	{
		InteractionActor = OtherActor;
		if (InteractionActor->Implements<UInteractable>())
		{
			IInteractable::Execute_BeginInteraction(InteractionActor, this);
		}
	}
}

void ATPlayerCharacter::OnInteractionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (InteractionActor)
	{
		if (InteractionActor->Implements<UInteractable>())
		{
			IInteractable::Execute_EndInteraction(InteractionActor);
		}
	}

	InteractionActor = nullptr;
}

void ATPlayerCharacter::SetupAbilityActorInfo()
{
	ATPlayerState* TPlayerState = GetPlayerState<ATPlayerState>();
	check(TPlayerState);

	TPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(TPlayerState, this);
	AbilitySystemComponent = TPlayerState->GetAbilitySystemComponent();
	Cast<UTAbilitySystemComponent>(AbilitySystemComponent)->BindAbilityActorInfo();
	AttributeSet = TPlayerState->GetAttributeSet();

	if (ATPlayerController* TPlayerController = Cast<ATPlayerController>(GetController()))
	{
		if (ATHUD* THUD = Cast<ATHUD>(TPlayerController->GetHUD()))
		{
			THUD->InitOverlayHUD(TPlayerController, TPlayerState, AbilitySystemComponent, AttributeSet);
			THUD->InitShopHUD(TPlayerController, TPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}


	AbilitySystemComponent->RegisterGameplayTagEvent(FTGameplayTags::Get().Effects_HitReact).AddUObject(
		this, &ThisClass::OnHitReactTagChanged);

	AbilitySystemComponent->RegisterGameplayTagEvent(FTGameplayTags::Get().State_Death).AddUObject(
		this, &ThisClass::OnDeathTagChanged);

	InitializeDefaultAttributes();
}

void ATPlayerCharacter::MulticastFlashParticles_Implementation() const
{
	if (IsValid(FlashNiagara))
	{
		FlashNiagara->Activate(true);
	}
}

void ATPlayerCharacter::OnHitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
}

void ATPlayerCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagara))
	{
		LevelUpNiagara->Activate(true);
	}
}

void ATPlayerCharacter::UpdateCameraZoom(float LengthDelta) const
{
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + LengthDelta, 650.f, 2000.f);
}
