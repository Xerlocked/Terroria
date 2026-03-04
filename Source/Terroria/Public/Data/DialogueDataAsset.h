// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogueDataAsset.generated.h"

// 선택지 액션 타입 정의
UENUM(BlueprintType)
enum class EDialogueActionType : uint8
{
	General UMETA(DisplayName = "일반 대화"),
	Quest UMETA(DisplayName = "퀘스트 수락/완료"),
	End UMETA(DisplayName = "대화 종료")
};

USTRUCT(BlueprintType)
struct FDialogueOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText OptionText; // 플레이어에게 보여질 선택지 텍스트

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	EDialogueActionType ActionType; // 이 선택지를 골랐을 때 일어날 행동

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue",
		meta = (EditCondition = "ActionType == EDialogueActionType::General"))
	FName NextNodeID; // 다음으로 넘어갈 대화 노드 ID (일반 대화일 경우)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue",
		meta = (EditCondition = "ActionType == EDialogueActionType::Quest"))
	FName QuestID; // 수락하거나 완료할 퀘스트 ID (퀘스트 관련일 경우)
};

USTRUCT(BlueprintType)
struct FDialogueNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText SpeakerName; // 말하는 사람 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (MultiLine = "true"))
	FText DialogueText; // NPC의 대사

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FDialogueOption> Options; // 플레이어가 고를 수 있는 선택지 배열
};

/**
 * 
 */
UCLASS()
class TERRORIA_API UDialogueDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// 노드 ID(FName)를 키값으로 사용하여 전체 대화 트리를 관리합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TMap<FName, FDialogueNode> DialogueTree;

	// 대화 시작 시 가장 먼저 보여줄 노드 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName StartNodeID;
};
