// Copyright 2025 @xerlock. All Rights Reserved.


#include "Component/DialogueComponent.h"

UDialogueComponent::UDialogueComponent()
{
	CurrentNodeID = NAME_None;
}

void UDialogueComponent::StartDialogue(UDialogueDataAsset* DialogueData)
{
	if (!IsValid(DialogueData))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 대화 데이터가 할당되지 않았습니다!"), *GetOwner()->GetName());
		return;
	}

	if (DialogueData->StartNodeID.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] 시작 대화 노드 ID가 설정되지 않았습니다!"), *GetOwner()->GetName());
		return;
	}

	GoToNextNode(DialogueData->StartNodeID);
}

void UDialogueComponent::SelectOption(const FDialogueOption& SelectedOption)
{
	// 플레이어가 선택한 옵션의 타입에 따라 분기 처리
	switch (SelectedOption.ActionType)
	{
	case EDialogueActionType::General:
		{
			// 일반 대화일 경우 다음 지정된 노드로 이동
			if (!SelectedOption.NextNodeID.IsNone())
			{
				GoToNextNode(SelectedOption.NextNodeID);
			}
			else
			{
				// 다음 노드가 지정되지 않았다면 대화 종료
				OnDialogueEnded.Broadcast();
			}
			break;
		}

	case EDialogueActionType::Quest:
		{
			// 퀘스트 시스템에 퀘스트 ID를 전달 (퀘스트 매니저가 이 델리게이트를 구독하여 처리하도록 위임)
			if (!SelectedOption.QuestID.IsNone())
			{
				OnQuestTriggered.Broadcast(SelectedOption.QuestID);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("퀘스트 타입의 선택지이지만 QuestID가 비어있습니다."));
			}

			// 퀘스트 수락 후 대화를 종료 (기획에 따라 특정 대화 노드로 이동하게끔 변경 가능)
			OnDialogueEnded.Broadcast();
			break;
		}

	case EDialogueActionType::End:
		{
			// 명시적인 대화 종료 처리
			CurrentNodeID = NAME_None;
			OnDialogueEnded.Broadcast();
			break;
		}

	default:
		{
			UE_LOG(LogTemp, Error, TEXT("처리할 수 없는 ActionType 입니다."));
			OnDialogueEnded.Broadcast();
			break;
		}
	}
}

void UDialogueComponent::GoToNextNode(FName NodeID)
{
	if (!IsValid(CurrentDialogueData))
	{
		return;
	}

	const FDialogueNode* FoundNode = CurrentDialogueData->DialogueTree.Find(NodeID);
	if (FoundNode)
	{
		CurrentNodeID = NodeID;
		OnDialogueUpdated.Broadcast(*FoundNode);
	}
	else
	{
		OnDialogueEnded.Broadcast();
	}
}
