// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/TCharacterClassDataAsset.h"
#include "UObject/Interface.h"
#include "CharacterData.generated.h"
class UAnimMontage;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCharacterData : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERRORIA_API ICharacterData
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetWeaponSocketLocation() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	virtual void Die() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevel() const;
};
