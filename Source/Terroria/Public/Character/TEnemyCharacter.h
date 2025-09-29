// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TCharacterBase.h"
#include "Interface/Highlight.h"
#include "TEnemyCharacter.generated.h"

UCLASS()
class TERRORIA_API ATEnemyCharacter : public ATCharacterBase, public IHighlight
{
	GENERATED_BODY()

public:
	ATEnemyCharacter();

	virtual void BeginPlay() override;

	//~Begin Highlight interface
	virtual void ActiveHighlightActor() override;
	virtual void DeactiveHighlightActor() override;
	//~End Highlight interface

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bActiveHighlight;

protected:
	virtual void SetupAbilityActorInfo() override;
};
