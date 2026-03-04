// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TCharacterBase.h"
#include "Interface/Interactable.h"
#include "TNonPlayerCharacter.generated.h"

class UWidgetComponent;
class UDialogueDataAsset;
class UDialogueComponent;
class UCameraComponent;

UCLASS()
class TERRORIA_API ATNonPlayerCharacter : public ATCharacterBase, public IInteractable
{
	GENERATED_BODY()

public:
	ATNonPlayerCharacter();

	virtual void BeginPlay() override;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual void BeginInteraction_Implementation(APawn* InstigatorPawn) override;

	virtual void EndInteraction_Implementation() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC|Data")
	FName NPCId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|Camera")
	TObjectPtr<UCameraComponent> InteractionCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|UI")
	TObjectPtr<UWidgetComponent> InteractionWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC|Dialogue")
	TObjectPtr<UDialogueDataAsset> DialogueDataAsset;
};
