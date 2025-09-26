// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPlayerController.generated.h"

class IHighlight;
class ATPlayerCharacter;
struct FInputActionValue;
class UNiagaraSystem;
class UInputAction;
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
	
	virtual void BeginPlay() override;

	virtual void PlayerTick(float DeltaTime) override;
	
protected:
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MappingContexts;

	UPROPERTY(EditAnywhere, Category="Input|Actions")
	UInputAction* DestClickAction;

	UPROPERTY(EditAnywhere, Category="Input|Actions")
	UInputAction* WheelAction;

	UPROPERTY(EditAnywhere, Category="Input|Actions")
	UInputAction* DashAction;
	
	UPROPERTY(EditAnywhere, Category="Effect")
	UNiagaraSystem* FXCursor;

	UPROPERTY(EditAnywhere, Category="Game|Properties")
	float ZoomDelta;
	
private:
	void DoClickStart();

	void DoWheel(const FInputActionValue& Value);

	void DoDash();

	void TraceCursor();
	
	UPROPERTY()
	TObjectPtr<ATPlayerCharacter> PossessedCharacter;

	TScriptInterface<IHighlight> LastActor;
	TScriptInterface<IHighlight> CurrentActor;
};	
