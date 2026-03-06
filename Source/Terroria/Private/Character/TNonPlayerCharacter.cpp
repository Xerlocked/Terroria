// Copyright 2025 @xerlock. All Rights Reserved.


#include "Character/TNonPlayerCharacter.h"

#include "AbilitySystem/TAbilitySystemComponent.h"
#include "AbilitySystem/TAbilitySystemLibrary.h"
#include "AbilitySystem/TAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Character/TPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "DialogueSystem/NPCDialogueComponent.h"
#include "DialogueSystem/PlayerDialogueComponent.h"
#include "QuestSystem/QuestGiverComponent.h"


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

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("InteractionWidgetComponent");
	InteractionWidgetComponent->SetupAttachment(GetRootComponent());

	DialogueComponent = CreateDefaultSubobject<UNPCDialogueComponent>("DialogueComponent");

	QuestGiverComponent = CreateDefaultSubobject<UQuestGiverComponent>("QuestGiverComponent");
}

void ATNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetupAbilityActorInfo();
	AddCharacterAbilities();
	UTAbilitySystemLibrary::GiveCommonAbilities(this, AbilitySystemComponent);

	InteractionWidgetComponent->SetVisibility(false);
}

void ATNonPlayerCharacter::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ATPlayerCharacter* PlayerCharacter = Cast<ATPlayerCharacter>(InstigatorPawn))
	{
		PlayerCharacter->GetLocalDialogueComponent()->TryInteract(this);
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

FVector ATNonPlayerCharacter::GetWeaponSocketLocation_Implementation(FName SocketName) const
{
	return GetMesh()->GetSocketLocation(SocketName);
}

int32 ATNonPlayerCharacter::GetPlayerLevel_Implementation() const
{
	return 5;
}

void ATNonPlayerCharacter::SetupAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UTAbilitySystemComponent>(AbilitySystemComponent)->BindAbilityActorInfo();

	InitializeDefaultAttributes();
}

void ATNonPlayerCharacter::InitializeDefaultAttributes() const
{
	UTAbilitySystemLibrary::InitializedDefaultAttributes(this, CharacterClass, 5.0f, AbilitySystemComponent);
}

void ATNonPlayerCharacter::HandleDeath_Implementation()
{
	Super::HandleDeath_Implementation();
}
