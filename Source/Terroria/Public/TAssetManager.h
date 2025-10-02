// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class TERRORIA_API UTAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UTAssetManager& Get();
	
protected:

	virtual void StartInitialLoading() override;
	
private:
};
