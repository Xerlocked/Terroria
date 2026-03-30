// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TerroriaBlueprintLibrary.generated.h"

class UBlackboardComponent;
class ATHUD;
class UCapsuleComponent;
/**
 * 
 */
UCLASS()
class TERRORIA_API UTerroriaBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Terroria|Helper")
	static FVector GetFurthestValidLocation(
		const UObject* WorldContextObject,
		const FVector& StartLocation, const FVector& EndLocation,
		const UCapsuleComponent* CapsuleComponent,
		const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
		const TArray<AActor*>& ActorsToIgnore,
		EDrawDebugTrace::Type DrawDebugType,
		bool bIgnoreSelf);

	UFUNCTION(BlueprintCallable, Category = "Terroria|Helper")
	static ATHUD* GetTHUD(const UObject* WorldContextObject);

private:
	static bool IsValidLocation(const UObject* WorldContextObject, const FVector& Location,
	                            const TArray<FHitResult>& HitResults);
};
