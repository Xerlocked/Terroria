// Copyright 2025 @xerlock. All Rights Reserved.


#include "UI/WidgetController/TOverlayWidgetController.h"

#include "AbilitySystem/TAbilitySystemComponent.h"
#include "AbilitySystem/TAttributeSet.h"
#include "AbilitySystem/Data/TAbilityDataAsset.h"
#include "AbilitySystem/Data/TLevelUpDataAsset.h"
#include "Character/TEnemyCharacter.h"
#include "Character/TPlayerCharacter.h"
#include "DialogueSystem/PlayerDialogueComponent.h"
#include "Player/TPlayerController.h"
#include "Player/TPlayerState.h"

void UTOverlayWidgetController::BroadcastInitialValues()
{
	const UTAttributeSet* Attributes = CastChecked<UTAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(Attributes->GetHealth());
	OnMaxHealthChanged.Broadcast(Attributes->GetMaxHealth());
	OnManaChanged.Broadcast(Attributes->GetMana());
	OnMaxManaChanged.Broadcast(Attributes->GetMaxMana());

	ATPlayerState* TPlayerState = CastChecked<ATPlayerState>(PlayerState);
	OnPlayerLevelChanged.Broadcast(TPlayerState->GetPlayerLevel());
	OnPlayerGoldChanged.Broadcast(TPlayerState->GetGold());
}

void UTOverlayWidgetController::BindCallbacksToDependencies()
{
	check(WidgetMessageDataTable);

	// PlayerState
	ATPlayerState* TPlayerState = CastChecked<ATPlayerState>(PlayerState);
	TPlayerState->OnXPChangedDelegate.AddUObject(this, &UTOverlayWidgetController::OnXPChanged);

	TPlayerState->OnLevelChangedDelegate.AddLambda
	([this](int32 InLevel)
		{
			OnPlayerLevelChanged.Broadcast(InLevel);
		}
	);

	TPlayerState->OnGoldChangedDelegate.AddLambda
	([this](int32 InGold)
		{
			OnPlayerGoldChanged.Broadcast(InGold);
		}
	);

	// PlayerController
	ATPlayerController* TPlayerController = CastChecked<ATPlayerController>(PlayerController);

	TPlayerController->OnTargeting.AddLambda([this](AActor* TargetActor)
	{
		OnTargetingActorChanged.Broadcast(TargetActor);
	});

	if (const ATPlayerCharacter* TPlayerCharacter = Cast<ATPlayerCharacter>(AbilitySystemComponent->GetAvatarActor()))
	{
		TPlayerCharacter->GetLocalDialogueComponent()->OnLocalDialogueStarted.AddLambda(
			[this](ACharacter* InPlayer, ACharacter* NPC)
			{
				OnDialogStarted.Broadcast(InPlayer, NPC);
			});

		TPlayerCharacter->GetLocalDialogueComponent()->OnLocalDialogueEnded.AddLambda([this]()
		{
			OnDialogueEnded.Broadcast();
		});

		TPlayerCharacter->GetLocalDialogueComponent()->OnLocalNodeChanged.AddLambda([this](const FDialogueNode& Node)
		{
			OnDialogueNodeChanged.Broadcast(Node);
		});
	}

	// ASC
	const UTAttributeSet* Attributes = CastChecked<UTAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});

	if (UTAbilitySystemComponent* ASC = Cast<UTAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (ASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(ASC);
		}
		else
		{
			ASC->AbilityGivenEvent.AddUObject(this, &UTOverlayWidgetController::OnInitializeStartupAbilities);
		}

		ASC->AbilityLevelChangedEvent.AddLambda(
			[this](const FGameplayTag& AbilityTag, int32 NewLevel)
			{
				OnAbilityLevelChanged.Broadcast(AbilityTag, NewLevel);
			}
		);

		ASC->AssetTagsEvent.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				for (const FGameplayTag& AssetTag : AssetTags)
				{
					if (AssetTag.MatchesTag(MessageTag))
					{
						const FWidgetMessageRow* Row = GetDataTableRowByTag<FWidgetMessageRow>(
							WidgetMessageDataTable, AssetTag);
						OnAssetTagMessage.Broadcast(*Row);
					}
				}
			}
		);
	}
}

void UTOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	const ATPlayerState* TPlayerState = CastChecked<ATPlayerState>(PlayerState);
	const UTLevelUpDataAsset* LevelUpInfo = TPlayerState->LevelUpInfo;

	checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out TPlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpsInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpsInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpsInformation[Level - 1].LevelUpRequirement;
		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UTOverlayWidgetController::OnInitializeStartupAbilities(UTAbilitySystemComponent* TAbilitySystemComponent)
{
	if (!TAbilitySystemComponent->bStartupAbilitiesGiven)
	{
		return;
	}

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, TAbilitySystemComponent](const FGameplayAbilitySpec& Spec)
	{
		FTAbilityData Data = AbilityDataAsset->FindAbilityDataByTag(
			TAbilitySystemComponent->GetAbilityTagFromSpec(Spec));
		Data.InputTag = TAbilitySystemComponent->GetInputTagFromSpec(Spec);
		OnAbilityDataDelegate.Broadcast(Data);
	});

	TAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}
