// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "Terroria.h"
#include "AbilitySystem/TAbilitySystemComponent.h"
#include "TPlayerController.generated.h"

class UTShopComponent;
class UGameplayInputQueueSystem;
class IInteractable;
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

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetingDelegate, AActor*);

UCLASS(Abstract)
class TERRORIA_API ATPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATPlayerController();

	virtual void BeginPlay() override;

	virtual void PlayerTick(float DeltaTime) override;

	virtual void OnRep_Pawn() override;

	/**
		 * Returns the controller's TAbilitySystemComponent.
		 * @return UTAbilitySystemComponent;
		 */
	UTAbilitySystemComponent* GetTASC();

	UFUNCTION(BlueprintCallable)
	ETerroriaCursor GetPlayerCursor() const { return PlayerCursorType; }

	UFUNCTION(BlueprintCallable)
	void SetPlayerCursor(ETerroriaCursor NewType) { PlayerCursorType = NewType; }

	UFUNCTION(BlueprintCallable)
	void SetIsPointerOverUI(bool bIsOver)
	{
		bIsPointerOverUI = bIsOver;
	}

	UTShopComponent* GetShopComponent() const { return ShopComponent; }

	FOnTargetingDelegate OnTargeting;

protected:
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MappingContexts;

	UPROPERTY(EditAnywhere, Category="Effect")
	UNiagaraSystem* FXCursor;

	UPROPERTY(EditAnywhere, Category="Game|Properties")
	float ZoomDelta;

	UPROPERTY(VisibleAnywhere, Category="Game|Cursor")
	ETerroriaCursor PlayerCursorType;

	UPROPERTY(VisibleAnywhere)
	USplineComponent* RouteSpline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UGameplayInputQueueSystem> InputQueueSystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTShopComponent> ShopComponent;

private:
	UFUNCTION()
	void OnInputConsumed(FGameplayTag InputTag);

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

	bool bIsPointerOverUI = false;

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
	float MinPressedThreshold = 0.5f;

	/** Arrival tolerance radius */
	float StopMovementRadius = 50.f;

	/** Moving to current destination */
	uint8 bMovingToDestination : 1;

	/** if true, target selected */
	uint8 bIsTargeting : 1;
	// ~End Click to Move
};
