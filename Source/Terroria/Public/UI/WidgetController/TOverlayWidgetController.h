// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TUserWidgetController.h"
#include "TOverlayWidgetController.generated.h"

struct FTAbilityData;
class UTAbilitySystemComponent;
class UTAbilityDataAsset;

USTRUCT(BlueprintType)
struct FWidgetMessageRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Tags")
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Display on message")
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName = "Widget")
	TSubclassOf<class UTUserWidget> MessageWidget;
};

class UTUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAssetTagMessageSignature, FWidgetMessageRow, WidgetMessageRow);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityDataSignature, const FTAbilityData&, AbilityData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetingActorChangedSignature, AActor*, TargetActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogStartedSignature, ACharacter*, InPlayer, ACharacter*, NPC);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNodeChangedSignature, const FDialogueNode&, Node);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEndedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestStatusChangedSignature, FName, QuestID, EQuestStatus, NewStatus);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectiveCompletedSignature, FName, QuestID, FName, ObjectiveID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnObjectiveUpdatedSignature, FName, QuestID, FName, ObjectiveID, int32,
                                               CurrentCount);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TERRORIA_API UTOverlayWidgetController : public UTUserWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;

	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Message")
	FOnAssetTagMessageSignature OnAssetTagMessage;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Message")
	FAbilityDataSignature OnAbilityDataDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Level")
	FOnPlayerStatusChangedSignature OnPlayerLevelChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Gold")
	FOnPlayerStatusChangedSignature OnPlayerGoldChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Targeting")
	FOnTargetingActorChangedSignature OnTargetingActorChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Abilities")
	FOnAbilityLevelChangedSignature OnAbilityLevelChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Dialogue")
	FOnDialogStartedSignature OnDialogStarted;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Dialogue")
	FOnNodeChangedSignature OnDialogueNodeChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Dialogue")
	FOnDialogueEndedSignature OnDialogueEnded;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Quest")
	FOnQuestStatusChangedSignature OnQuestStatusChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Quest")
	FOnObjectiveCompletedSignature OnObjectiveCompleted;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Quest")
	FOnObjectiveUpdatedSignature OnObjectiveUpdated;

protected:
	template <typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	void OnXPChanged(int32 NewXP) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|DataTable")
	TObjectPtr<UDataTable> WidgetMessageDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|DataTable")
	TObjectPtr<UTAbilityDataAsset> AbilityDataAsset;

	void OnInitializeStartupAbilities(UTAbilitySystemComponent* TAbilitySystemComponent);
};

template <typename T>
T* UTOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
