// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TCharacterBase.h"
#include "Interface/Highlight.h"
#include "UI/WidgetController/TOverlayWidgetController.h"
#include "TEnemyCharacter.generated.h"

class USphereComponent;
class UWidgetComponent;

UCLASS()
class TERRORIA_API ATEnemyCharacter : public ATCharacterBase, public IHighlight
{
	GENERATED_BODY()

public:
	ATEnemyCharacter();

	virtual void BeginPlay() override;

	//~Begin Highlight interface
	virtual ETerroriaCursor GetCursorType() override;
	virtual void ActiveHighlightActor() override;
	virtual void DeactiveHighlightActor() override;
	//~End Highlight interface

	virtual FVector GetWeaponSocketLocation_Implementation(FName SocketName) const override;
	virtual int32 GetPlayerLevel_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|AI")
	float AttackRange = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|AI")
	float LeashRadius = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Combat")
	float LifeSpan = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Status")
	FString DisplayCharacterName;

protected:
	virtual void SetupAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() const override;

	virtual void SpawnDropItem() override;

	virtual void HandleDeath_Implementation() override;

	virtual void OnDeathTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Status")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Status")
	bool bIsDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|UI")
	TObjectPtr<UWidgetComponent> HealthWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<USphereComponent> InteractionCollision;

	UPROPERTY
	(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
};
