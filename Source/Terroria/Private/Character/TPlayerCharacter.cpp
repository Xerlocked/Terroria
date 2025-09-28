// Copyright 2025 @xerlock. All Rights Reserved.

#include "Public/Character/TPlayerCharacter.h"

#include "AbilitySystemComponent.h"
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
}

void ATPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	SetupAbilityActorInfo();
}

void ATPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	SetupAbilityActorInfo();
}

void ATPlayerCharacter::SetupAbilityActorInfo()
{
	ATPlayerState* TPlayerState = GetPlayerState<ATPlayerState>();
	check(TPlayerState);
	
	TPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(TPlayerState, this);
	AbilitySystemComponent = TPlayerState->GetAbilitySystemComponent();
	AttributeSet = TPlayerState->GetAttributeSet();

	if (ATPlayerController* TPlayerController = Cast<ATPlayerController>(GetController()))
	{
		if (ATHUD* THUD = Cast<ATHUD>(TPlayerController->GetHUD()))
		{
			THUD->InitOverlayHUD(TPlayerController, TPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

void ATPlayerCharacter::UpdateCameraZoom(float LengthDelta) const
{
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + LengthDelta, 650.f, 2000.f);
}

