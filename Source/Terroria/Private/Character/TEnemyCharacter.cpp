// Copyright 2025 @xerlock. All Rights Reserved.

#include "Character/TEnemyCharacter.h"
#include "AbilitySystem/TAbilitySystemComponent.h"

ATEnemyCharacter::ATEnemyCharacter()
{

}

void ATEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
