// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Main log category used across the project */
DECLARE_LOG_CATEGORY_EXTERN(LogTerroria, Log, All);

#define CUSTOM_DEPTH_RED 250

#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1

UENUM(BlueprintType)
enum class ETerroriaCursor : uint8
{
	Normal,
	Attack,
	Interact,
	Unavailable,
	Working,
	Busy
};