// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPlayerController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS(Abstract)
class TERRORIA_API ATPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATPlayerController();

protected:

	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MappingContexts;

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
};
