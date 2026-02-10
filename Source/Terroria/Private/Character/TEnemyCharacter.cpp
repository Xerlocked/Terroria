// Copyright 2025 @xerlock. All Rights Reserved.

#include "Character/TEnemyCharacter.h"

#include "Terroria.h"
#include "TGameplayTags.h"
#include "AbilitySystem/TAbilitySystemComponent.h"
#include "AbilitySystem/TAbilitySystemLibrary.h"
#include "AbilitySystem/TAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/TUserWidget.h"

ATEnemyCharacter::ATEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UTAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UTAttributeSet>(TEXT("AttributeSet"));

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>("HealthWidget");
	HealthWidget->SetupAttachment(GetRootComponent());
}

void ATEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	SetupAbilityActorInfo();
	AddCharacterAbilities();
	UTAbilitySystemLibrary::GiveCommonAbilities(this, AbilitySystemComponent);

	if (UTUserWidget* TUserWidget = Cast<UTUserWidget>(HealthWidget->GetUserWidgetObject()))
	{
		TUserWidget->SetWidgetController(this);
	}

	if (const UTAttributeSet* TAttributes = Cast<UTAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TAttributes->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TAttributes->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		OnHealthChanged.Broadcast(TAttributes->GetHealth());
		OnMaxHealthChanged.Broadcast(TAttributes->GetMaxHealth());
	}
}

void ATEnemyCharacter::ActiveHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ATEnemyCharacter::DeactiveHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
}

FVector ATEnemyCharacter::GetWeaponSocketLocation_Implementation() const
{
	return GetMesh()->GetSocketLocation(WeaponSocketName);
}

int32 ATEnemyCharacter::GetPlayerLevel_Implementation() const
{
	return Level;
}

void ATEnemyCharacter::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void ATEnemyCharacter::SetupAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UTAbilitySystemComponent>(AbilitySystemComponent)->BindAbilityActorInfo();

	InitializeDefaultAttributes();
}

void ATEnemyCharacter::InitializeDefaultAttributes() const
{
	UTAbilitySystemLibrary::InitializedDefaultAttributes(this, CharacterClass, 1.0f, AbilitySystemComponent);
}
