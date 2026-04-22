// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TERRORIA_API USoundManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayBackgroundSound(USoundBase* NewSound, float FadeTime = 1.f);

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void StopBackgroundSound(float FadeTime = 1.f);

private:
	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioSoundComponent;
};
