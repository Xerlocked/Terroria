// Copyright 2025 @xerlock. All Rights Reserved.


#include "TAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "TGameplayTags.h"

UTAssetManager& UTAssetManager::Get()
{
	check(GEngine);
	return *Cast<UTAssetManager>(GEngine->AssetManager);
}

void UTAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FTGameplayTags::InitializeNativeGameplayTags();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
