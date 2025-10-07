// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TCharacterBase.h"
#include "TPlayerCharacter.generated.h"

struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;

UCLASS()
class TERRORIA_API ATPlayerCharacter : public ATCharacterBase
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

	virtual int32 GetPlayerLevel() const override;

	virtual FVector GetWeaponSocketLocation() const override;

private:
	virtual void SetupAbilityActorInfo() override;
	
public:
	UCameraComponent* GetPlayerCameraComponent() const { return PlayerCamera; }

	void UpdateCameraZoom(float LengthDelta) const;
};
