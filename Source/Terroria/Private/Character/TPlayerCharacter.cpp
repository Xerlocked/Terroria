// Copyright 2025 @xerlock. All Rights Reserved.

#include "Public/Character/TPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "TGameplayTags.h"
#include "AbilitySystem/TAbilitySystemComponent.h"
#include "AbilitySystem/Data/TLevelUpDataAsset.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/TPlayerController.h"
#include "Player/TPlayerState.h"
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

	CharacterClass = ECharacterClass::Magician;
}

void ATPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	SetupAbilityActorInfo();
	AddCharacterAbilities();
}

void ATPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	SetupAbilityActorInfo();
}

int32 ATPlayerCharacter::GetPlayerLevel_Implementation() const
{
	const ATPlayerState* TPlayerState = GetPlayerState<ATPlayerState>();
	check(TPlayerState);

	return TPlayerState->GetPlayerLevel();
}

FVector ATPlayerCharacter::GetWeaponSocketLocation_Implementation() const
{
	return GetMesh()->GetSocketLocation(WeaponSocketName);
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
		}
	}


	AbilitySystemComponent->RegisterGameplayTagEvent(FTGameplayTags::Get().Effects_HitReact).AddUObject(
		this, &ThisClass::OnHitReactTagChanged);

	InitializeDefaultAttributes();
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
