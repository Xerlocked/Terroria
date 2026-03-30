// Copyright 2025 @xerlock. All Rights Reserved.

#include "Character/TEnemyCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Terroria.h"
#include "TGameplayTags.h"
#include "AbilitySystem/TAbilitySystemComponent.h"
#include "AbilitySystem/TAbilitySystemLibrary.h"
#include "AbilitySystem/TAttributeSet.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/Itemable.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/TUserWidget.h"

ATEnemyCharacter::ATEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	AbilitySystemComponent = CreateDefaultSubobject<UTAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UTAttributeSet>(TEXT("AttributeSet"));

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>("HealthWidget");
	HealthWidget->SetupAttachment(GetRootComponent());

	InteractionCollision = CreateDefaultSubobject<USphereComponent>("InteractionCollision");
	InteractionCollision->SetupAttachment(GetRootComponent());
	InteractionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	InteractionCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
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

ETerroriaCursor ATEnemyCharacter::GetCursorType()
{
	return ETerroriaCursor::Attack;
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

FVector ATEnemyCharacter::GetWeaponSocketLocation_Implementation(FName SocketName) const
{
	return GetMesh()->GetSocketLocation(SocketName);
}

int32 ATEnemyCharacter::GetPlayerLevel_Implementation() const
{
	return Level;
}

void ATEnemyCharacter::SetupAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UTAbilitySystemComponent>(AbilitySystemComponent)->BindAbilityActorInfo();

	AbilitySystemComponent->RegisterGameplayTagEvent(FTGameplayTags::Get().State_Death).AddUObject(
		this, &ThisClass::OnDeathTagChanged);

	InitializeDefaultAttributes();
}

void ATEnemyCharacter::InitializeDefaultAttributes() const
{
	UTAbilitySystemLibrary::InitializedDefaultAttributes(this, CharacterClass, 1.0f, AbilitySystemComponent);
}

void ATEnemyCharacter::SpawnDropItem()
{
	for (const FDropItem& Item : DropTables)
	{
		if ((FMath::FRand() <= Item.DropChance) && Item.ItemClass)
		{
			int32 AmountToDrop = FMath::RandRange(Item.MinQuantity, Item.MaxQuantity);

			FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 50.f);

			FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

			AActor* SpawnedItem = GetWorld()->SpawnActorDeferred<AActor>(
				Item.ItemClass,
				SpawnTransform,
				this,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
			);

			if (SpawnedItem)
			{
				if (SpawnedItem->Implements<UItemable>())
				{
					IItemable::Execute_SetAmount(SpawnedItem, AmountToDrop);
				}

				UGameplayStatics::FinishSpawningActor(SpawnedItem, SpawnTransform);
			}
		}
	}
}

void ATEnemyCharacter::HandleDeath_Implementation()
{
	if (HasAuthority())
	{
		if (bIsDead)
		{
			return;
		}

		bIsDead = true;

		if (GetController())
		{
			GetController()->SetLifeSpan(LifeSpan);
			GetController()->UnPossess();
		}

		SpawnDropItem();

		SetLifeSpan(LifeSpan);

		if (HealthWidget)
		{
			HealthWidget->DestroyComponent();
		}

		InteractionCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ATEnemyCharacter::OnDeathTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		if (AAIController* AIC = Cast<AAIController>(GetController()))
		{
			UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(AIC->GetBrainComponent());
			if (BTComponent)
			{
				BTComponent->StopTree();
			}
		}
		HandleDeath();
	}
}
