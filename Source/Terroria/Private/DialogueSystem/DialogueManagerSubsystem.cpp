// Copyright 2025 @xerlock. All Rights Reserved.


#include "DialogueSystem/DialogueManagerSubsystem.h"

#include "AbilitySystemComponent.h"
#include "Character/TPlayerCharacter.h"
#include "DialogueSystem/Data/DialogueData.h"
#include "QuestSystem/QuestManagerSubsystem.h"

bool UDialogueManagerSubsystem::StartDialogue(ACharacter* Player, ACharacter* NPC,
                                              UDialogueData* DialogueData)
{
	if (!Player || !NPC || !DialogueData)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartDialog: 유효하지 않은 인자"));
		return false;
	}

	// 이미 대화 중이면 차단
	if (IsInDialogue(Player))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s 는 이미 대화 중입니다."), *Player->GetName());
		return false;
	}

	// 세션 생성
	FDialogueSession NewSession;
	NewSession.DialogueData = DialogueData;
	NewSession.NPC = NPC;
	NewSession.Player = Player;
	NewSession.CurrentNodeID = DialogueData->RootNodeID;

	ActiveSessions.Add(Player, NewSession);

	OnDialogueStarted.Broadcast(Player, NPC);

	// 루트 노드 조건 평가 후 첫 노드로 이동
	NextToNode(Player, DialogueData->RootNodeID);

	return true;
}

void UDialogueManagerSubsystem::SelectResponse(ACharacter* Player, int32 Index)
{
	FDialogueSession* Session = GetSession(Player);
	if (!Session)
	{
		return;
	}

	const FDialogueNode* CurrentNode = FindNode(Session->DialogueData, Session->CurrentNodeID);
	if (!CurrentNode)
	{
		return;
	}

	// 유효한 선택지만 필터링 (조건 통과한 것들)
	TArray<const FDialogueResponse*> ValidResponses;
	for (const FDialogueResponse& Response : CurrentNode->Responses)
	{
		if (EvaluateConditions(Player, Response.Conditions))
		{
			ValidResponses.Add(&Response);
		}
	}

	if (!ValidResponses.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("잘못된 ResponseIndex: %d"), Index);
		return;
	}

	const FDialogueResponse* SelectedResponse = ValidResponses[Index];

	// 선택지에 달린 이벤트 처리 (퀘스트 수락 등)
	ProcessEvents(Player, SelectedResponse->Events);

	// 다음 노드가 없으면 대화 종료
	if (SelectedResponse->NextNodeID == NAME_None)
	{
		EndDialogue(Player);
		return;
	}

	NextToNode(Player, SelectedResponse->NextNodeID);
}

void UDialogueManagerSubsystem::EndDialogue(ACharacter* Player)
{
	if (!IsInDialogue(Player))
	{
		return;
	}

	ActiveSessions.Remove(Player);
	OnDialogueEnded.Broadcast(Player);
}

bool UDialogueManagerSubsystem::GetCurrentNode(ACharacter* Player, FDialogueNode& OutNode) const
{
	const FDialogueNode* Node = GetCurrentNodeInternal(Player);
	if (!Node)
	{
		return false;
	}

	OutNode = *Node;
	return true;
}

const FDialogueNode* UDialogueManagerSubsystem::GetCurrentNodeInternal(ACharacter* Player) const
{
	const FDialogueSession* Session = GetSession(Player);
	if (!Session)
	{
		return nullptr;
	}

	return FindNode(Session->DialogueData, Session->CurrentNodeID);
}

bool UDialogueManagerSubsystem::IsInDialogue(ACharacter* Player) const
{
	return ActiveSessions.Contains(Player);
}

int32 UDialogueManagerSubsystem::GetActiveSessionCountForNPC(ACharacter* NPC) const
{
	int32 Count = 0;
	for (const auto& Pair : ActiveSessions)
	{
		if (Pair.Value.NPC == NPC)
		{
			Count++;
		}
	}
	return Count;
}

bool UDialogueManagerSubsystem::EvaluateConditions(ACharacter* Player,
                                                   const TArray<FDialogueCondition>& Conditions)
{
	if (Conditions.IsEmpty())
	{
		return true;
	}

	UQuestManagerSubsystem* QuestManager =
		Player->GetGameInstance()->GetSubsystem<UQuestManagerSubsystem>();

	for (const FDialogueCondition& Condition : Conditions)
	{
		bool bResult = false;

		switch (Condition.ConditionType)
		{
		case EDialogueConditionType::None:
			bResult = true;
			break;

		case EDialogueConditionType::QuestStatus:
			//bResult = QuestManager &&
			// 	QuestManager->GetQuestStatus(Condition.TargetQuestID) == Condition.RequiredStatus;
			break;

		case EDialogueConditionType::HasItem:
			// bResult = Player->GetInventory()->HasItem(Condition.RequiredItemID);
			break;

		case EDialogueConditionType::PlayerLevel:
			// bResult = Player->GetLevel() >= Condition.RequiredLevel;
			break;

		case EDialogueConditionType::GameplayTag:
			bResult = false;

			if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Player))
			{
				bResult = ASCInterface->GetAbilitySystemComponent()->HasMatchingGameplayTag(Condition.RequiredTag);
			}
			break;

		case EDialogueConditionType::DialoguePlayed:
			// bResult = PlayedDialogIDs.Contains(Condition.TargetDialogID);
			break;

		case EDialogueConditionType::RelationshipLevel:
			// bResult = Player->GetRelationshipLevel(Condition.TargetNPCID) >= Condition.RequiredLevel;
			break;
		}

		// bNegate 로 조건 반전 가능
		if (Condition.bNegate)
		{
			bResult = !bResult;
		}

		// 하나라도 실패하면 즉시 false (AND 조건)
		if (!bResult)
		{
			return false;
		}
	}

	return true;
}

void UDialogueManagerSubsystem::ProcessEvents(ACharacter* Player, const TArray<FDialogueEvent>& Events)
{
	if (Events.IsEmpty())
	{
		return;
	}

	UQuestManagerSubsystem* QuestManager =
		Player->GetGameInstance()->GetSubsystem<UQuestManagerSubsystem>();

	for (const FDialogueEvent& Event : Events)
	{
		switch (Event.EventType)
		{
		case EDialogueEventType::None:
			break;

		case EDialogueEventType::StartQuest:
			if (QuestManager)
			{
				// QuestManager->AcceptQuest(Event.QuestID);
			}
			break;

		case EDialogueEventType::CompleteObjective:
			if (QuestManager)
			{
				// QuestManager->UpdateObjective(Event.QuestID, Event.ObjectiveID, 1);
			}
			break;

		case EDialogueEventType::CompleteQuest:
			if (QuestManager)
			{
				// QuestManager->CompleteQuest(Event.QuestID);
			}
			break;

		case EDialogueEventType::GiveItem:
			// Player->GetInventory()->AddItem(Event.ItemID);
			break;

		case EDialogueEventType::TakeItem:
			// Player->GetInventory()->RemoveItem(Event.ItemID);
			break;

		case EDialogueEventType::AddGameplayTag:
			if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Player))
			{
				ASCInterface->GetAbilitySystemComponent()->AddLooseGameplayTag(Event.PlayTag);
			}
			break;

		case EDialogueEventType::FireCustomEvent:
			// Blueprint나 다른 시스템에서 바인딩해서 처리
			// OnCustomEventFired.Broadcast(Player, Event.CustomEventName);
			break;
		}
	}
}

const FDialogueNode* UDialogueManagerSubsystem::FindNode(const UDialogueData* DialogueData, FName NodeID) const
{
	if (!DialogueData)
	{
		return nullptr;
	}

	return DialogueData->DialogueNodes.FindByPredicate([&NodeID](const FDialogueNode& Node)
	{
		return Node.NodeID == NodeID;
	});
}

void UDialogueManagerSubsystem::NextToNode(ACharacter* Player, FName NextNodeID)
{
	FDialogueSession* Session = GetSession(Player);
	if (!Session)
	{
		return;
	}

	const FDialogueNode* NextNode = FindNode(Session->DialogueData, NextNodeID);
	if (!NextNode)
	{
		// 노드를 못 찾으면 대화 종료
		UE_LOG(LogTemp, Warning, TEXT("NodeID %s 를 찾을 수 없어 대화 종료"), *NextNodeID.ToString());
		EndDialogue(Player);
		return;
	}

	// 노드 진입 조건 확인
	if (!EvaluateConditions(Player, NextNode->NodeConditions))
	{
		UE_LOG(LogTemp, Log, TEXT("Node %s 진입 조건 미충족"), *NextNodeID.ToString());
		EndDialogue(Player);
		return;
	}

	Session->CurrentNodeID = NextNodeID;

	// 노드 진입 시 이벤트 처리
	ProcessEvents(Player, NextNode->NodeEvents);

	OnNodeChanged.Broadcast(Player, *NextNode);
}

FDialogueSession* UDialogueManagerSubsystem::GetSession(const ACharacter* Player)
{
	return ActiveSessions.Find(Player);
}

const FDialogueSession* UDialogueManagerSubsystem::GetSession(const ACharacter* Player) const
{
	return ActiveSessions.Find(Player);
}
