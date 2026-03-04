// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/ShopItemTableRow.h"
#include "UI/WidgetController/TUserWidgetController.h"
#include "TShopWidgetController.generated.h"

class UTAbilityDataAsset;
struct FShopItemTableRow;
class UTShopComponent;

// Delegate for broadcasting shop items list to the UI
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShopItemsUpdatedSignature, const TArray<FShopItemTableRow>&, ShopItems);

// Delegate for single item update (after purchase)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShopItemPurchasedSignature, const FShopItemContext, ItemContext);

// Delegate for purchase result feedback
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPurchaseResultSignature, const FGameplayTag&, ItemTag, bool, bSuccess);

/**
 * 상점 UI의 데이터 바인딩을 전담하는 MVC 형태의 WidgetController 모델입니다.
 */
UCLASS(BlueprintType, Blueprintable)
class TERRORIA_API UTShopWidgetController : public UTUserWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable, Category = "Shop|Upgrade")
	void RequestPurchaseItem(const FGameplayTag& ItemTag);

	// Delegates for Widget binding
	UPROPERTY(BlueprintAssignable, Category = "Shop|Events")
	FOnShopItemsUpdatedSignature OnShopItemsUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Shop|Events")
	FOnShopItemPurchasedSignature OnShopItemPurchased;

	UPROPERTY(BlueprintAssignable, Category = "Shop|Events")
	FOnPurchaseResultSignature OnPurchaseResult;

	UPROPERTY(BlueprintAssignable, Category = "Shop|Events")
	FOnPlayerStatusChangedSignature OnGoldChanged;

	UPROPERTY(BlueprintAssignable, Category = "Shop|Events")
	FOnAbilityLevelChangedSignature OnAbilityLevelChanged;

protected:
	UPROPERTY()
	TObjectPtr<UTShopComponent> PlayerShopComponent;
};
