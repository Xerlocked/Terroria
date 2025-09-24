// Copyright 2025 @xerlock. All Rights Reserved.

#include "Public/Character/TPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Terroria.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/TPlayerState.h"

ATPlayerCharacter::ATPlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 450.f;
	SpringArm->TargetOffset = FVector(0.0f, 80.f, 120.f);
	
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);
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

void ATPlayerCharacter::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementDirection = Value.Get<FVector2D>();
	
	DoMove(MovementDirection.X, MovementDirection.Y);
}

void ATPlayerCharacter::LookInput(const FInputActionValue& Value)
{
	FVector2D LookDirection = Value.Get<FVector2D>();

	DoAim(LookDirection.X, LookDirection.Y);
}

void ATPlayerCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ATPlayerCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ATPlayerCharacter::DoJumpStart()
{
	Jump();
}

void ATPlayerCharacter::DoJumpEnd()
{
	StopJumping();
}

// Called to bind functionality to input
void ATPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		/* Jump */
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATPlayerCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATPlayerCharacter::DoJumpEnd);

		/* Move */
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPlayerCharacter::MoveInput);

		/* Look */
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPlayerCharacter::LookInput);
	}
	else
	{
		UE_LOG(LogTerroria, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void ATPlayerCharacter::SetupAbilityActorInfo()
{
	ATPlayerState* TPlayerState = GetPlayerState<ATPlayerState>();
	check(TPlayerState);
	
	TPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(TPlayerState, this);
	AbilitySystemComponent = TPlayerState->GetAbilitySystemComponent();
	AttributeSet = TPlayerState->GetAttributeSet();
}

