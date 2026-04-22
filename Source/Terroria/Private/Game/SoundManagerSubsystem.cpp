// Copyright 2025 @xerlock. All Rights Reserved.


#include "Game/SoundManagerSubsystem.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void USoundManagerSubsystem::PlayBackgroundSound(USoundBase* NewSound, float FadeTime)
{
	if (!NewSound)
	{
		return;
	}

	if (AudioSoundComponent && AudioSoundComponent->IsPlaying())
	{
		if (AudioSoundComponent->Sound == NewSound)
		{
			return; // 이미 재생 중인 사운드와 동일한 경우, 아무 작업도 하지 않음
		}

		AudioSoundComponent->FadeOut(FadeTime, 0.f);
	}

	AudioSoundComponent = UGameplayStatics::CreateSound2D(GetWorld(), NewSound, 1.0f, 1.0f, 0.0f, nullptr, true, false);

	if (AudioSoundComponent)
	{
		AudioSoundComponent->FadeIn(FadeTime, 1.f);
	}
}

void USoundManagerSubsystem::StopBackgroundSound(float FadeTime)
{
	if (AudioSoundComponent && AudioSoundComponent->IsPlaying())
	{
		AudioSoundComponent->FadeOut(FadeTime, 0.f);
		AudioSoundComponent = nullptr;
	}
}
