// Copyright 2025 @xerlock. All Rights Reserved.

#include "Character/TEnemyCharacter.h"

#include "Terroria.h"
#include "AbilitySystem/TAbilitySystemComponent.h"

ATEnemyCharacter::ATEnemyCharacter()
{

}

void ATEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupAbilityActorInfo();
}

void ATEnemyCharacter::ActiveHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ATEnemyCharacter::DeactiveHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
}

int32 ATEnemyCharacter::GetPlayerLevel() const
{
	return Level;
}

void ATEnemyCharacter::SetupAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UTAbilitySystemComponent>(AbilitySystemComponent)->BindAbilityActorInfo();
}
