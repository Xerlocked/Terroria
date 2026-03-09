// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "TCharacterBase.h"
#include "Interface/PlayerInterface.h"
#include "QuestSystem/Interface/QuestReceiver.h"
#include "TPlayerCharacter.generated.h"

class UQuestReceiverComponent;
class UPlayerDialogueComponent;
class UDialogueComponent;
class USphereComponent;
class UGameplayInputQueueSystem;
struct FGameplayTag;
class UNiagaraComponent;
struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;

UCLASS()
class TERRORIA_API ATPlayerCharacter : public ATCharacterBase, public IPlayerInterface, public IQuestReceiver
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> PlayerCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> LevelUpNiagara;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> FlashNiagara;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> InteractionCollision;

public:
	ATPlayerCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	// Begin CharacterData Interface
	virtual FVector GetWeaponSocketLocation_Implementation(FName SocketName) const override;
	virtual int32 GetPlayerLevel_Implementation() const override;

	virtual int32 GetComboIndex_Implementation() override;
	virtual void AdvanceCombo_Implementation() override;
	virtual void ResetCombo_Implementation() override;
	// End CharacterData Interface

	// Begin Player Interface
	virtual void LevelUP_Implementation() override;
	virtual void AddToXP_Implementation(int32 NewXP) override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 FindLevelForXP_Implementation(int32 XP) const override;
	virtual int32 GetAttributePoint_Implementation() const override;
	virtual ATPlayerController* GetPlayerController_Implementation() const override;

	virtual int32 GetCurrentGold_Implementation() const override;
	virtual bool SpendGold_Implementation(int32 Amount) override;
	virtual int32 GetItemLevel_Implementation(FGameplayTag EventTag) const override;

	virtual void UpgradeAbilityByTag_Implementation(const FGameplayTag& AbilityTag) override;
	// End Player Interface

	// Begin Quest Receiver

	virtual UQuestReceiverComponent* GetQuestReceiverComponent() const override;

	virtual void AddExperience(int32 Amount) override;
	virtual void AddCurrency(int32 Amount) override;
	virtual void AddItem(FName ItemID, int32 Quantity) override;
	virtual void AddGameplayTag(FGameplayTag Tag) override;

	// End Quest Receiver

	void ProcessInteraction();

	AActor* GetInteractionActor() const { return InteractionActor; }

protected:
	virtual void HandleDeath_Implementation() override;

	UFUNCTION()
	void OnInteractionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                               const FHitResult& SweepResult);
	UFUNCTION()
	void OnInteractionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	virtual void SetupAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFlashParticles() const;

	void OnHitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

public:
	UCameraComponent* GetPlayerCameraComponent() const { return PlayerCamera; }

	void UpdateCameraZoom(float LengthDelta) const;

	UPlayerDialogueComponent* GetLocalDialogueComponent() const { return LocalDialogueComponent; }

	UQuestReceiverComponent* GetQuestReceiverComponent() { return QuestReceiverComponent; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Mapping")
	TMap<FGameplayTag, FGameplayAttribute> TagToAttributeMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPlayerDialogueComponent> LocalDialogueComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UQuestReceiverComponent> QuestReceiverComponent;

private:
	UPROPERTY(VisibleAnywhere, Replicated)
	int32 CurrentComboIndex = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Combo")
	int32 MaxComboCount = 2;

	UPROPERTY()
	TObjectPtr<AActor> InteractionActor;
};
