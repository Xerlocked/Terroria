// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TBaseAreaEffector.h"
#include "TSphereAreaEffector.generated.h"

class USphereComponent;

UCLASS()
class TERRORIA_API ATSphereAreaEffector : public ATBaseAreaEffector
{
	GENERATED_BODY()

public:
	ATSphereAreaEffector();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<USphereComponent> SphereComponent;
};
