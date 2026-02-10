// Copyright 2025 @xerlock. All Rights Reserved.


#include "UI/WidgetController/TOverlayWidgetController.h"

#include "AbilitySystem/TAbilitySystemComponent.h"
#include "AbilitySystem/TAttributeSet.h"
#include "AbilitySystem/Data/TAbilityDataAsset.h"
#include "AbilitySystem/Data/TLevelUpDataAsset.h"
#include "Player/TPlayerState.h"

void UTOverlayWidgetController::BroadcastInitialValues()
{
	const UTAttributeSet* Attributes = CastChecked<UTAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(Attributes->GetHealth());
	OnMaxHealthChanged.Broadcast(Attributes->GetMaxHealth());
	OnManaChanged.Broadcast(Attributes->GetMana());
	OnMaxManaChanged.Broadcast(Attributes->GetMaxMana());
}

void UTOverlayWidgetController::BindCallbacksToDependencies()
{
	check(WidgetMessageDataTable);
	
	ATPlayerState* TPlayerState = CastChecked<ATPlayerState>(PlayerState);
	TPlayerState->OnXPChangedDelegate.AddUObject(this, &UTOverlayWidgetController::OnXPChanged);

	TPlayerState->OnLevelChangedDelegate.AddLambda
	([this](int32 InLevel)
		{
			OnPlayerLevelChanged.Broadcast(InLevel);
		}
	);
	
	const UTAttributeSet* Attributes = CastChecked<UTAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnManaChanged.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
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
		
		ASC->AssetTagsEvent.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				for (const FGameplayTag& AssetTag : AssetTags)
				{
					if (AssetTag.MatchesTag(MessageTag))
					{
						const FWidgetMessageRow* Row = GetDataTableRowByTag<FWidgetMessageRow>(WidgetMessageDataTable, AssetTag);
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
	if (!TAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, TAbilitySystemComponent](const FGameplayAbilitySpec& Spec)
	{
		FTAbilityData Data = AbilityDataAsset->FindAbilityDataByTag(TAbilitySystemComponent->GetAbilityTagFromSpec(Spec));
		Data.InputTag = TAbilitySystemComponent->GetInputTagFromSpec(Spec);
		OnAbilityDataDelegate.Broadcast(Data);
	});
	
	TAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}
