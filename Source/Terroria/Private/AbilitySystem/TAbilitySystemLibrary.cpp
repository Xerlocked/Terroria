// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/TAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/TPlayerState.h"
#include "UI/HUD/THUD.h"
#include "UI/WidgetController/TUserWidgetController.h"

UTOverlayWidgetController* UTAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ATHUD* THUD = Cast<ATHUD>(PC->GetHUD()))
		{
			ATPlayerState* PS = PC->GetPlayerState<ATPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams Params(PC, PS, ASC, AS);
			return THUD->GetOverlayWidgetController(Params);
		}
	}

	return nullptr;
}

UTAttributeMenuWidgetController* UTAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ATHUD* THUD = Cast<ATHUD>(PC->GetHUD()))
		{
			ATPlayerState* PS = PC->GetPlayerState<ATPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams Params(PC, PS, ASC, AS);
			return THUD->GetAttributeMenuWidgetController(Params);
		}
	}

	return nullptr;
}
