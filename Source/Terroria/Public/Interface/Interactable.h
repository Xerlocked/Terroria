// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE()
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERRORIA_API IInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Intract")
	void BeginInteraction(APawn* InstigatorPawn);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Intract")
	void EndInteraction();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Intract")
	void Interact(APawn* InstigatorPawn);
};
