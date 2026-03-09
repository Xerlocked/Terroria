// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestSystem/QuestSystemTypes.h"
#include "QuestData.generated.h"

/**
 * 
 */
UCLASS()
class TERRORIA_API UQuestData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName QuestName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText QuestTitle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText QuestDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FQuestObjective> QuestObjectives;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FQuestReward> QuestRewards;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> PreRequiredQuests;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bRequireReturnToNPC = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsMainQuest = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimeLimit = 0.0f;
};
