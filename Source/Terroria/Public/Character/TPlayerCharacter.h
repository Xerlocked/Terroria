// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TCharacterBase.h"
#include "Interface/PlayerInterface.h"
#include "TPlayerCharacter.generated.h"

struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;

UCLASS()
class TERRORIA_API ATPlayerCharacter : public ATCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* PlayerCamera;

public:
	ATPlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	// Begin CharacterData Interface
	virtual FVector GetWeaponSocketLocation() const override;
	virtual int32 GetPlayerLevel_Implementation() const override;
	// End CharacterData Interface
	
	// Begin Player Interface
	virtual void LevelUP_Implementation() override;
	virtual void AddToXP_Implementation(int32 NewXP) override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 FindLevelForXP_Implementation(int32 XP) const override;
	// End Player Interface
	
private:
	virtual void SetupAbilityActorInfo() override;
	
public:
	UCameraComponent* GetPlayerCameraComponent() const { return PlayerCamera; }

	void UpdateCameraZoom(float LengthDelta) const;
};
