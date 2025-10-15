// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/TAbilitySystemLibrary.h"

#include "Game/TGameModeBase.h"
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

UTAttributeMenuWidgetController* UTAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
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

void UTAbilitySystemLibrary::InitializedDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	ATGameModeBase* TGameMode = Cast<ATGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (TGameMode == nullptr) return;

	// Get character class info from gamebase
	const AActor* Avatar = ASC->GetAvatarActor();
	UTCharacterClassDataAsset* CharacterClassDataAsset = TGameMode->CharacterClassInfo;
	FCharacterClassInfo ClassDefaultInfo = CharacterClassDataAsset->GetCharacterClassInfo(CharacterClass);

	// Primary attributes set
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(Avatar);
	FGameplayEffectSpecHandle PrimarySpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data.Get());

	// Secondary attributes set
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(Avatar);
	const FGameplayEffectSpecHandle SecondarySpecHandle = ASC->MakeOutgoingSpec(CharacterClassDataAsset->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data.Get());
	
	// Vital attributes set
	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(Avatar);
	const FGameplayEffectSpecHandle VitalSpecHandle = ASC->MakeOutgoingSpec(CharacterClassDataAsset->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get());
}
