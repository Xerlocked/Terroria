// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ShopItemTableRow.h"
#include "TShopComponent.generated.h"


UCLASS(ClassGroup=(Shop), meta=(BlueprintSpawnableComponent))
class TERRORIA_API UTShopComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTShopComponent();

	bool PurchaseItem(AActor* Buyer, const FShopItemTableRow& ItemData);

	bool PurchaseItem(AActor* Buyer, const FGameplayTag& ItemEventTag);

	TArray<FShopItemTableRow> GetAllShopItems() const;

	FShopItemContext GetShopItemContext(const FGameplayTag& ItemEventTag) const;

	UFUNCTION(BlueprintCallable, Category = "Shop|Upgrade")
	int32 GetPurchaseCount(const FGameplayTag& EventTag) const;

private:
	int32 CalculatePrice(const FShopItemTableRow& ItemData, int32 CurrentLevel) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shop|Data")
	TObjectPtr<UDataTable> ShopDataTable;

	UPROPERTY(VisibleAnywhere, Category = "Shop|Data" /* SaveGame */)
	TMap<FGameplayTag, int32> PurchaseCountMap;
};
