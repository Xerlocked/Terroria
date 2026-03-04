// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Itemable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UItemable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERRORIA_API IItemable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void SetAmount(int32 InAmount);
};
