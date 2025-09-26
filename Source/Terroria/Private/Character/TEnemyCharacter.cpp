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

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ATEnemyCharacter::ActiveHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	UE_LOG(LogTemp, Warning, TEXT("Active Highlight Actor"));

	bActiveHighlight = true;
}

void ATEnemyCharacter::DeactiveHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	UE_LOG(LogTemp, Warning, TEXT("Deactive Highlight Actor"));

	bActiveHighlight = false;
}
