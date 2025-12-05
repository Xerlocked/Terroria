// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/TAbilitySystemLibrary.h"

#include "Game/TGameModeBase.h"
#include "Interface/CharacterData.h"
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
	UTCharacterClassDataAsset* CharacterClassDataAsset = GetCharacterClassDataAsset(WorldContextObject);
	if (CharacterClassDataAsset == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[TAbilitySystemLibrary - 51:46] CharacterClassDataAsset is nullptr"));
		return;
	}
	
	// Get character class info from gamebase
	const AActor* Avatar = ASC->GetAvatarActor();
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

void UTAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	ATGameModeBase* TGameMode = Cast<ATGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (TGameMode == nullptr) return;

	UTCharacterClassDataAsset* CharacterClassDataAsset = TGameMode->CharacterClassInfo;

	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassDataAsset->CommonAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCharacterData>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICharacterData::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UTCharacterClassDataAsset* UTAbilitySystemLibrary::GetCharacterClassDataAsset(const UObject* WorldContextObject)
{
	ATGameModeBase* TGameMode = Cast<ATGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (TGameMode == nullptr) return nullptr;
	return TGameMode->CharacterClassInfo;
}

int32 UTAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 Level)
{
	UTCharacterClassDataAsset* CharacterClassInfo = GetCharacterClassDataAsset(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassInfo& Info = CharacterClassInfo->GetCharacterClassInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(Level);

	return static_cast<int32>(XPReward);
}
