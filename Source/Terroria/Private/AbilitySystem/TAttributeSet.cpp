// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/TAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UTAttributeSet::UTAttributeSet()
{
	InitHealth(50.0f);
	InitMana(100.0f);
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
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, CastSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, HealthDrain, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	
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
	}
}

void UTAttributeSet::GetGameplayEffectProperty(const FGameplayEffectModCallbackData& Data, FEffectProperty& Property)
{
	Property.ContextHandle = Data.EffectSpec.GetContext();

	Property.InstigatorASC = Property.ContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Property.InstigatorASC) && Property.InstigatorASC->AbilityActorInfo.IsValid() && Property.InstigatorASC->AbilityActorInfo->AvatarActor.IsValid())
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

void UTAttributeSet::OnRep_CastSpeed(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, CastSpeed, OldValue);
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

void UTAttributeSet::OnRep_HealthDrain(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, HealthDrain, OldValue);
}

void UTAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, Armor, OldValue);
}

void UTAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTAttributeSet, BlockChance, OldValue);
}
