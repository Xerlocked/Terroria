// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Interface/StatusInterface.h"
#include "TPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;


UCLASS()
class TERRORIA_API ATPlayerState : public APlayerState, public IAbilitySystemInterface, public IStatusInterface
{
	GENERATED_BODY()

public:
	ATPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UAttributeSet* GetAttributeSet() { return AttributeSet; }

	virtual int32 GetPlayerLevel() const override;
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level, Category = "Character|Status")
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level();
};
