// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TUserWidgetController.h"
#include "TAttributeMenuWidgetController.generated.h"

class UTAttributeDataAsset;
struct FTAttributeData;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeDataSignature, const FTAttributeData&, Data);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TERRORIA_API UTAttributeMenuWidgetController : public UTUserWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeDataSignature OnAttributeDataDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerStatusChangedSignature AttributeChangedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTAttributeDataAsset> AttributeDataAsset;

private:
	void BroadcastOnAttributeData(const FGameplayTag& Tag, const FGameplayAttribute& Attribute) const;
};
