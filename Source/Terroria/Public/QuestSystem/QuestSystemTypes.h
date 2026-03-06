// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestSystemTypes.generated.h"

UENUM(BlueprintType)
enum class EQuestStatus : uint8
{
	NotStarted UMETA(DisplayName = "NotStarted", ToolTip = "퀘스트를 아직 수락하지 않은 상태"),
	Active UMETA(DisplayName = "Active", ToolTip = "퀘스트를 수락하고 진행 중인 상태"),
	ObjectiveComplete UMETA(DisplayName = "ObjectiveComplete", ToolTip = "일부 목표는 완료했지만 퀘스트는 아직 완료되지 않은 상태"),
	Completed UMETA(DisplayName = "Completed", ToolTip = "모든 목표를 완료하고 퀘스트가 종료된 상태"),
	Failed UMETA(DisplayName = "Failed", ToolTip = "시간 초과 또는 조건 미충족으로 퀘스트가 실패한 상태"),
	Abandoned UMETA(DisplayName = "Abandoned", ToolTip = "플레이어가 직접 퀘스트를 포기한 상태"),
};

UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
	KillTarget UMETA(DisplayName = "KillTarget", ToolTip = "특정 적 또는 몬스터를 지정한 수만큼 처치하는 목표"),
	CollectItem UMETA(DisplayName = "CollectItem", ToolTip = "특정 아이템을 지정한 수량만큼 수집하는 목표"),
	ReachLocation UMETA(DisplayName = "ReachLocation", ToolTip = "지정된 장소 또는 구역에 도달하는 목표"),
	TalkToNPC UMETA(DisplayName = "TalkToNPC", ToolTip = "특정 NPC와 대화를 완료하는 목표"),
	Escort UMETA(DisplayName = "Escort", ToolTip = "특정 NPC 또는 대상을 목적지까지 호위하는 목표"),
	Protect UMETA(DisplayName = "Protect", ToolTip = "특정 NPC 또는 오브젝트를 일정 시간 동안 보호하는 목표"),
	Interact UMETA(DisplayName = "Interact", ToolTip = "특정 오브젝트 또는 장치와 상호작용하는 목표"),
	Survive UMETA(DisplayName = "Survive", ToolTip = "지정된 시간 동안 생존하는 목표"),
};

USTRUCT(BlueprintType)
struct FQuestObjective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName ObjectiveID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FText ObjectiveText = FText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	EObjectiveType ObjectiveType = EObjectiveType::KillTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FGameplayTag TargetTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int32 RequiredCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int32 CurrentCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	bool bIsOptional = false;
};

USTRUCT(BlueprintType)
struct FRewardItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	FName ItemID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	int32 Quantity = 1;
};

USTRUCT(BlueprintType)
struct FQuestReward
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	int32 Experience = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	int32 Currency = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	TArray<FRewardItem> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
	TArray<FGameplayTag> RewardTags;
};
