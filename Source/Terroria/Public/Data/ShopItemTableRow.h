// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "ShopItemTableRow.generated.h"

/**
 * @struct FShopItemTableRow
 * @brief Represents the data structure used for defining shop item entries in a data table.
 *
 * This structure is derived from the FTableRowBase class and is used to define configurable
 * shop item properties for use in a shop system. It is equipped to work with Unreal Engine's
 * data table functionality and is marked as BlueprintType, allowing it to be utilized in
 * Blueprints.
 *
 * @details
 * - CategoryTag: Identifies the category of the item using a gameplay tag.
 * - EventTag: Associates the item with a specific event using a gameplay tag.
 * - ItemName: Holds the display name of the shop item.
 * - ItemDescription: Contains a description or details about the shop item.
 * - PriceCurve: Points to a curve asset defining price variations for this item over time or other factors.
 */
USTRUCT(BlueprintType)
struct FShopItemTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Category")
	FGameplayTag CategoryTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Event")
	FGameplayTag EventTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Item")
	FText ItemName = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Item")
	FText ItemDescription = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Item")
	TSoftObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Item")
	FLinearColor IconTintColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Item")
	FScalableFloat PriceCurve = FScalableFloat();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Item")
	int32 MaxLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop|Item")
	float Magnitude = 1.f;
};

USTRUCT(BlueprintType)
struct FShopItemContext
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	FGameplayTag EventTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	int32 CurrentPurchaseCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	int32 CurrentPrice = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	bool bIsMaxLevel = false;
};
