// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "TShopDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FAttributeShopInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BaseCost = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat CostMultiplier = FScalableFloat();
};

USTRUCT(BlueprintType)
struct FAbilityShopInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BaseCost = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat CostMultiplier = FScalableFloat();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxLevel = 5;
};

USTRUCT()
struct FShopData
{
	GENERATED_BODY()
};

/**
 * 
 */
UCLASS(BlueprintType)
class TERRORIA_API UTShopDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shop")
	TArray<FAttributeShopInfo> AttributeShopInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shop")
	TArray<FAbilityShopInfo> AbilityShopInfo;

	FAttributeShopInfo FindAttributeShopInfoByTag(const FGameplayTag& AttributeTag);
	FAbilityShopInfo FindAbilityShopInfoByTag(const FGameplayTag& AbilityTag);
};
