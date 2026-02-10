// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "TBaseAreaEffector.generated.h"

UCLASS()
class TERRORIA_API ATBaseAreaEffector : public AActor
{
	GENERATED_BODY()

public:
	ATBaseAreaEffector();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void ApplyEffectToTarget();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<UPrimitiveComponent> OverlapComponent;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle EffectSpecHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timer")
	float TickInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timer")
	float Duration = 5.0f;
};
