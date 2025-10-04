// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/TAbilitySystemComponent.h"
#include "TPlayerController.generated.h"

class USplineComponent;
class UTAbilitySystemComponent;
class UTGameplayInput;
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
	
	UPROPERTY(EditAnywhere, Category="Effect")
	UNiagaraSystem* FXCursor;

	UPROPERTY(EditAnywhere, Category="Game|Properties")
	float ZoomDelta;

	UPROPERTY(VisibleAnywhere)
	USplineComponent* RouteSpline;
	
private:
	void TraceCursor();
	
	void MovePlayerToDestination();

	void DoWheel(const FInputActionValue& Value);
	
	void PressedAbilityAction(const FInputActionValue& Value, FGameplayTag Tag);

	void ReleasedAbilityAction(const FInputActionValue& Value, FGameplayTag Tag);

	void HeldAbilityAction(const FInputActionValue& Value, FGameplayTag Tag);
	
	UPROPERTY()
	TObjectPtr<ATPlayerCharacter> PossessedCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UTGameplayInput> InputContext;
	
	UPROPERTY()
	TObjectPtr<UTAbilitySystemComponent> TAbilitySystemComponent;

	FHitResult CursorTraceHit;
	
	// ~Begin Highlight
	TScriptInterface<IHighlight> LastActor;
	TScriptInterface<IHighlight> CurrentActor;
	// ~End Highlight
	
	// ~Begin Click to Move
	/** Cached location, Spline, or last clicked location. */
	FVector CachedDestination;

	/** Mouse press time */
	float MousePressTime = 0.f;

	/** Threshold for minimum required time for a pressed reaction */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float MinPressedThreshold = 0.02f;

	/** Arrival tolerance radius */
	float StopMovementRadius = 50.f;

	/** Moving to current destination */
	uint8 bMovingToDestination: 1;

	/** if true, target selected */
	uint8 bIsTargeting: 1;
	// ~End Click to Move

private:
	/**
	 * Returns the controller's TAbilitySystemComponent.
	 * @return UTAbilitySystemComponent;
	 */
	UTAbilitySystemComponent* GetTASC();
};