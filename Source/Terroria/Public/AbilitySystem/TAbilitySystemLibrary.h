// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TAbilitySystemLibrary.generated.h"

class UTAttributeMenuWidgetController;
class UTOverlayWidgetController;
/**
 * 
 */
UCLASS()
class TERRORIA_API UTAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|WidgetController")
	static UTOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|WidgetController")
	static UTAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
};
