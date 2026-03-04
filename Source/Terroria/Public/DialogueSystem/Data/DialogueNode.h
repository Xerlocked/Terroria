// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueSystem/DialogueSystemTypes.h"
#include "DialogueNode.generated.h"

USTRUCT(BlueprintType)
struct FDialogueResponse
{
	GENERATED_BODY()

	// 선택지에 표시될 텍스트 (예: "네, 돕겠습니다" / "아니오, 관심없어요")
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ResponseText = FText();

	// 이 선택지를 골랐을 때 이동할 다음 노드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextNodeID = NAME_None;

	// 이 선택지가 표시되기 위한 조건들
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDialogueCondition> Conditions;

	// 이 선택지를 골랐을 때 발동되는 이벤트들
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDialogueEvent> Events;
};

USTRUCT(BlueprintType)
struct FDialogueNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NodeID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText NodeText = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDialogueResponse> Responses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDialogueCondition> NodeConditions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDialogueEvent> NodeEvents;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundCue> AudioCue = nullptr;
};
