// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DialogueSystemTypes.generated.h"

enum class EQuestStatus : uint8;

UENUM(BlueprintType)
enum class EDialogueConditionType : uint8
{
	None UMETA(DisplayName = "None", ToolTip = "조건 없음 - 항상 표시"),
	QuestStatus UMETA(DisplayName = "QuestStatus", ToolTip = "특정 퀘스트가 지정된 상태인지 확인"),
	HasItem UMETA(DisplayName = "HasItem", ToolTip = "플레이어 인벤토리에 특정 아이템이 있는지 확인"),
	PlayerLevel UMETA(DisplayName = "PlayerLevel", ToolTip = "플레이어 레벨이 일정 수치 이상/이하인지 확인"),
	GameplayTag UMETA(DisplayName = "GameplayTag", ToolTip = "플레이어나 NPC가 특정 GameplayTag를 보유하는지 확인"),
	DialoguePlayed UMETA(DisplayName = "DialoguePlayed", ToolTip = "이전에 특정 대화를 재생했는지 확인"),
	RelationshipLevel UMETA(DisplayName = "RelationshipLevel", ToolTip = "해당 NPC와의 친밀도가 특정 수치 이상인지 확인"),
};

UENUM(BlueprintType)
enum class EDialogueEventType : uint8
{
	None UMETA(DisplayName = "None", ToolTip = "이벤트 없음 - 일상 대화에 사용"),
	StartQuest UMETA(DisplayName = "StartQuest", ToolTip = "퀘스트를 새로 시작 - 대화 중 퀘스트 수락 시 사용"),
	CompleteObjective UMETA(DisplayName = "CompleteObjective", ToolTip = "특정 퀘스트 목표를 완료 처리 - TalkToNPC 목표 달성 등에 사용"),
	CompleteQuest UMETA(DisplayName = "CompleteQuest", ToolTip = "퀘스트 전체를 완료 처리 - 퀘스트 마무리 대화에서 사용"),
	GiveItem UMETA(DisplayName = "GiveItem", ToolTip = "플레이어에게 아이템 지급 - 보상 아이템, 단서 아이템 전달에 사용"),
	TakeItem UMETA(DisplayName = "TakeItem", ToolTip = "플레이어의 아이템을 회수 - 수집 퀘스트 납품 등에 사용"),
	AddGameplayTag UMETA(DisplayName = "AddGameplayTag", ToolTip = "플레이어/NPC에 GameplayTag 부여 - 특정 정보 습득 플래그 등에 사용"),
	FireCustomEvent UMETA(DisplayName = "FireCustomEvent", ToolTip = "커스텀 이벤트 발동 - 컷씬, 레벨 이벤트, AI 상태 변경 등 확장 포인트")
};

USTRUCT(BlueprintType)
struct FDialogueCondition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDialogueConditionType ConditionType = EDialogueConditionType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TargetQuestID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EQuestStatus RequiredStatus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag RequiredTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bNegate = false;
};

USTRUCT(BlueprintType)
struct FDialogueEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDialogueEventType EventType = EDialogueEventType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName QuestID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ObjectiveID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag PlayTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName CustomEventName = NAME_None;
};
