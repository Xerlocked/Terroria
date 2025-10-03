// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "TGameplayInput.generated.h"

USTRUCT(BlueprintType)
struct FGameplayInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTags = FGameplayTag();
};


/**
 * 
 */
UCLASS()
class TERRORIA_API UTGameplayInput : public UDataAsset
{
	GENERATED_BODY()

public:

	const UInputAction* FindInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound = false);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameplayInputAction> AbilityInputActions;
};
