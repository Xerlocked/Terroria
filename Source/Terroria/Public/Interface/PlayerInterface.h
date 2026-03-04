// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

class ATPlayerController;

UINTERFACE(MinimalAPI, BlueprintType)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERRORIA_API IPlayerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 XP) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoint() const;

	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 NewXP);

	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InPlayerLevel);

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InAttributePoints);

	UFUNCTION(BlueprintNativeEvent)
	void LevelUP();

	//~Begin Shop
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface|Shop")
	int32 GetCurrentGold() const;

	// @Note: 입력 파라미터로 양수 값만 입력해주세요.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface|Shop")
	bool SpendGold(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface|Shop")
	int32 GetItemLevel(FGameplayTag EventTag) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface|Shop")
	void UpgradeAbilityByTag(const FGameplayTag& AbilityTag);

	//~End Shop

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ATPlayerController* GetPlayerController() const;
};
