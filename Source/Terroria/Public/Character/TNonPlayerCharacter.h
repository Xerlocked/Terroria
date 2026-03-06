// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TCharacterBase.h"
#include "Interface/Interactable.h"
#include "TNonPlayerCharacter.generated.h"

class UQuestGiverComponent;
class UNPCDialogueComponent;
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

	virtual FVector GetWeaponSocketLocation_Implementation(FName SocketName) const override;
	virtual int32 GetPlayerLevel_Implementation() const override;

protected:
	virtual void SetupAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void HandleDeath_Implementation() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC|Data")
	FName NPCId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC|Camera")
	TObjectPtr<UCameraComponent> InteractionCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|UI")
	TObjectPtr<UWidgetComponent> InteractionWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Dialogue")
	TObjectPtr<UNPCDialogueComponent> DialogueComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Quest")
	TObjectPtr<UQuestGiverComponent> QuestGiverComponent;
};
