// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TCharacterBase.h"
#include "AbilitySystem/Data/TCharacterClassDataAsset.h"
#include "Interface/Highlight.h"
#include "UI/WidgetController/TOverlayWidgetController.h"
#include "TEnemyCharacter.generated.h"

class UWidgetComponent;

UCLASS()
class TERRORIA_API ATEnemyCharacter : public ATCharacterBase, public IHighlight
{
	GENERATED_BODY()

public:
	ATEnemyCharacter();

	virtual void BeginPlay() override;

	//~Begin Highlight interface
	virtual void ActiveHighlightActor() override;
	virtual void DeactiveHighlightActor() override;
	//~End Highlight interface

	virtual FVector GetWeaponSocketLocation_Implementation() const override;
	virtual int32 GetPlayerLevel_Implementation() const override;

	virtual void Die() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Combat")
	float PrimaryAttackRange = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Combat")
	float LifeSpan = 5.f;

protected:
	virtual void SetupAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Status")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|UI")
	TObjectPtr<UWidgetComponent> HealthWidget;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
};
