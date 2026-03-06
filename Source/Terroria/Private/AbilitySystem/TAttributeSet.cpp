// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/TAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "TGameplayTags.h"
#include "AbilitySystem/TAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/CharacterData.h"
#include "Interface/PlayerInterface.h"
#include "Net/UnrealNetwork.h"

UTAttributeSet::UTAttributeSet()
{
	const FTGameplayTags& GameplayTags = FTGameplayTags::Get();

	/* Primary Attributes */
	TagsToAttributes.Add(GameplayTags.Attribute_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Primary_Agility, GetAgilityAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Primary_Luck, GetLuckAttribute);

	/* Secondary Attributes */
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_AttackPower, GetAttackPowerAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_AttackSpeed, GetAttackSpeedAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_MagicPower, GetMagicPowerAttribute);

	/* Vital Attributes */
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_MaxMana, GetMaxManaAttribute);

	/* Unique Attributes */
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_MovementSpeed, GetMovementSpeedAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_CDR, GetCooldownReductionAttribute);
}

void UTAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, Agility, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, Luck, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, MagicPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, CooldownReduction, COND_None, REPNOTIFY_Always);
}

void UTAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxMana());

		UE_LOG(LogTemp, Log, TEXT("Health value %f"), NewValue);
	}

	if (Attribute == GetMovementSpeedAttribute())
	{
		AActor* OwningActor = GetOwningActor();
		ACharacter* Character = Cast<ACharacter>(OwningActor);

		if (Character && Character->GetCharacterMovement())
		{
			Character->GetCharacterMovement()->MaxWalkSpeed = NewValue;

			UE_LOG(LogTemp, Log, TEXT("MaxWalkSpeed Value: %f"), NewValue);
		}
	}
}

void UTAttributeSet::GetGameplayEffectProperty(const FGameplayEffectModCallbackData& Data, FEffectProperty& Property)
{
	Property.ContextHandle = Data.EffectSpec.GetContext();

	Property.InstigatorASC = Property.ContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Property.InstigatorASC) && Property.InstigatorASC->AbilityActorInfo.IsValid() && Property.InstigatorASC
		->AbilityActorInfo->AvatarActor.IsValid())
	{
		Property.InstigatorAvatarActor = Property.InstigatorASC->AbilityActorInfo->AvatarActor.Get();
		Property.InstigatorController = Property.InstigatorASC->AbilityActorInfo->PlayerController.Get();

		if (Property.InstigatorAvatarActor != nullptr && Property.InstigatorController == nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Property.InstigatorAvatarActor))
			{
				Property.InstigatorController = Pawn->GetController();
			}
		}

		if (Property.InstigatorController)
		{
			Property.InstigatorCharacter = Cast<ACharacter>(Property.InstigatorController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Property.TargetASC = &Data.Target;
		Property.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Property.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Property.TargetCharacter = Cast<ACharacter>(Property.TargetAvatarActor);
	}
}


void UTAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperty EffectProperty;
	GetGameplayEffectProperty(Data, EffectProperty);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f;

			// HitReact 이벤트 전송 (데미지 표기 및 피격 반응용)
			// 사망 여부와 관계없이 데미지 수치를 전달하기 위해 실행합니다.
			FGameplayEventData Payload;
			Payload.EventTag = FTGameplayTags::Get().Effects_HitReact;
			Payload.EventMagnitude = LocalIncomingDamage;
			Payload.Instigator = EffectProperty.InstigatorAvatarActor;
			Payload.Target = EffectProperty.TargetAvatarActor;
			Payload.ContextHandle = EffectProperty.ContextHandle;

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				EffectProperty.TargetAvatarActor,
				Payload.EventTag,
				Payload
			);

			if (bFatal)
			{
				if (ICharacterData* CharacterData = Cast<ICharacterData>(EffectProperty.TargetAvatarActor))
				{
					CharacterData->Die();
				}
				SendXPEvent(EffectProperty);
				SendDeathEvent(EffectProperty);
			}
		}
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		const float LocalIncomingXP = GetIncomingXP();
		SetIncomingXP(0.f);

		if (EffectProperty.InstigatorCharacter->Implements<UPlayerInterface>())
		{
			const int32 CurrentLevel = ICharacterData::Execute_GetPlayerLevel(EffectProperty.InstigatorCharacter);
			const int32 CurrentXP = IPlayerInterface::Execute_GetXP(EffectProperty.InstigatorCharacter);

			const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(
				EffectProperty.InstigatorCharacter, CurrentXP + LocalIncomingXP);
			const int32 NumLevelUps = NewLevel - CurrentLevel;
			if (NumLevelUps > 0)
			{
				const int32 AttributePointsReward = IPlayerInterface::Execute_GetAttributePointsReward(
					EffectProperty.InstigatorCharacter, CurrentLevel);

				IPlayerInterface::Execute_AddToPlayerLevel(EffectProperty.InstigatorCharacter, NumLevelUps);
				IPlayerInterface::Execute_AddToAttributePoints(EffectProperty.InstigatorCharacter,
				                                               AttributePointsReward);

				bNeedMaxHealth = true;
				bNeedMaxMana = true;

				IPlayerInterface::Execute_LevelUP(EffectProperty.InstigatorCharacter);
			}

			IPlayerInterface::Execute_AddToXP(EffectProperty.InstigatorCharacter, LocalIncomingXP);
		}
	}
}

void UTAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bNeedMaxHealth)
	{
		SetHealth(GetMaxHealth());
		bNeedMaxHealth = false;
	}

	if (Attribute == GetMaxManaAttribute() && bNeedMaxMana)
	{
		SetMana(GetMaxMana());
		bNeedMaxMana = false;
	}
}

void UTAttributeSet::SendXPEvent(const FEffectProperty& Property)
{
	if (Property.TargetCharacter->Implements<UCharacterData>())
	{
		const int32 Level = ICharacterData::Execute_GetPlayerLevel(Property.TargetCharacter);
		ECharacterClass CharacterClass = ICharacterData::Execute_GetCharacterClass(Property.TargetCharacter);
		const int32 XPReward = UTAbilitySystemLibrary::GetXPRewardForClassAndLevel(
			Property.TargetCharacter, CharacterClass, Level);

		const FTGameplayTags& GameplayTags = FTGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attribute_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Property.InstigatorCharacter,
		                                                         GameplayTags.Attribute_Meta_IncomingXP, Payload);
	}
}

void UTAttributeSet::SendDeathEvent(const FEffectProperty& Property)
{
	if (Property.InstigatorCharacter && Property.TargetASC)
	{
		FGameplayTagContainer TargetTags;
		Property.TargetASC->GetOwnedGameplayTags(TargetTags);
		FGameplayTag KillEventTag = FTGameplayTags::Get().Event_Kill;

		for (const FGameplayTag& Tag : TargetTags)
		{
			if (Tag.MatchesTag(KillEventTag))
			{
				FGameplayEventData Payload;
				Payload.EventTag = Tag;
				Payload.EventMagnitude = 1.0f;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Property.InstigatorCharacter, Tag, Payload);
				break;
			}
		}
	}
}


void UTAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, Strength, OldValue);
}

void UTAttributeSet::OnRep_Agility(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, Agility, OldValue);
}

void UTAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, Intelligence, OldValue);
}

void UTAttributeSet::OnRep_Luck(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, Luck, OldValue);
}

void UTAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, AttackPower, OldValue);
}

void UTAttributeSet::OnRep_MagicPower(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, MagicPower, OldValue);
}

void UTAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, AttackSpeed, OldValue);
}

void UTAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, MovementSpeed, OldValue);
}

void UTAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, CriticalHitChance, OldValue);
}

void UTAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, CriticalHitDamage, OldValue);
}

void UTAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, Health, OldValue);
}

void UTAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, MaxHealth, OldValue);
}

void UTAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, HealthRegeneration, OldValue);
}

void UTAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, Mana, OldValue);
}

void UTAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, MaxMana, OldValue);
}

void UTAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, ManaRegeneration, OldValue);
}

void UTAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, Armor, OldValue);
}

void UTAttributeSet::OnRep_CooldownReduction(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, CooldownReduction, OldValue);
}
