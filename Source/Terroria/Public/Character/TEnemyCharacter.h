// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TCharacterBase.h"
#include "TEnemyCharacter.generated.h"

UCLASS()
class TERRORIA_API ATEnemyCharacter : public ATCharacterBase
{
	GENERATED_BODY()

public:
	ATEnemyCharacter();

	virtual void BeginPlay() override;
};
