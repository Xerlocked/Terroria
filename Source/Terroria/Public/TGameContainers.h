// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TGameContainers.generated.h"

USTRUCT(BlueprintType)
struct FDropItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropChance = 1.0; // 0.0 ~ 1.0

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinQuantity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxQuantity = 2;
};
