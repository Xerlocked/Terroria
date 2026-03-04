// Copyright 2025 @xerlock. All Rights Reserved.


#include "Character/TNonPlayerCharacter.h"

#include "AbilitySystem/TAbilitySystemComponent.h"
#include "AbilitySystem/TAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Character/TPlayerCharacter.h"
#include "Component/DialogueComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"


ATNonPlayerCharacter::ATNonPlayerCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	AbilitySystemComponent = CreateDefaultSubobject<UTAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UTAttributeSet>(TEXT("AttributeSet"));

	InteractionCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InteractionCamera"));
	InteractionCamera->SetupAttachment(GetRootComponent());
}

void ATNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InteractionWidgetComponent->SetVisibility(false);
}

void ATNonPlayerCharacter::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ATPlayerCharacter* PlayerCharacter = Cast<ATPlayerCharacter>(InstigatorPawn))
	{
		PlayerCharacter->GetLocalDialogueComponent()->StartDialogue(DialogueDataAsset);
	}
}

void ATNonPlayerCharacter::BeginInteraction_Implementation(APawn* InstigatorPawn)
{
	InteractionWidgetComponent->SetVisibility(true);
}

void ATNonPlayerCharacter::EndInteraction_Implementation()
{
	InteractionWidgetComponent->SetVisibility(false);
}
