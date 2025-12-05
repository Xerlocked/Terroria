// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "TCharacterClassDataAsset.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Warrior,
	Ranger,
	Magician
};

USTRUCT(BlueprintType)
struct FCharacterClassInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Primary")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	FScalableFloat XPReward = FScalableFloat();
};

/**
 * 
 */
UCLASS()
class TERRORIA_API UTCharacterClassDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Class")
	TMap<ECharacterClass, FCharacterClassInfo> CharacterClassInformation;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;
	
	FCharacterClassInfo GetCharacterClassInfo(const ECharacterClass CharacterClass);
};
