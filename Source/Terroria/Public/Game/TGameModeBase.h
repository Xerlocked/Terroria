// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TGameModeBase.generated.h"

class UTCharacterClassDataAsset;
/**
 * 
 */
UCLASS()
class TERRORIA_API ATGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UTCharacterClassDataAsset> CharacterClassInfo;
};
