// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueNode.h"
#include "Engine/DataAsset.h"
#include "DialogueData.generated.h"

/**
 * 
 */
UCLASS()
class TERRORIA_API UDialogueData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DialogueName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName RootNodeID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDialogueNode> DialogueNodes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsRepeatable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> VoiceAsset;
};
