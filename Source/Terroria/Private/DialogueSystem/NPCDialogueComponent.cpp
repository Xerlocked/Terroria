// Copyright 2025 @xerlock. All Rights Reserved.


#include "DialogueSystem/NPCDialogueComponent.h"

#include "DialogueSystem/DialogueManagerSubsystem.h"
#include "GameFramework/Character.h"
#include "QuestSystem/QuestManagerSubsystem.h"


UNPCDialogueComponent::UNPCDialogueComponent()
{
}

void UNPCDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UDialogueManagerSubsystem* DialogueManager = GetWorld()->GetSubsystem<UDialogueManagerSubsystem>())
	{
		DialogueManager->OnDialogueStarted.AddDynamic(this, &UNPCDialogueComponent::OnDialogueStarted);
		DialogueManager->OnDialogueEnded.AddDynamic(this, &UNPCDialogueComponent::OnDialogueEnded);
	}
}

void UNPCDialogueComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UDialogueManagerSubsystem* DialogueManager = GetWorld()->GetSubsystem<UDialogueManagerSubsystem>())
	{
		DialogueManager->OnDialogueStarted.RemoveDynamic(this, &UNPCDialogueComponent::OnDialogueStarted);
		DialogueManager->OnDialogueEnded.RemoveDynamic(this, &UNPCDialogueComponent::OnDialogueEnded);
	}

	Super::EndPlay(EndPlayReason);
}


UDialogueData* UNPCDialogueComponent::GetActiveDialogue(ACharacter* Player)
{
	if (!Player)
	{
		return nullptr;
	}

	// ConditionalDialogs를 Priority 내림차순으로 정렬 후 평가
	TArray<FConditionalDialogue> SortedDialogs = ConditionalDialogues;
	SortedDialogs.Sort([](const FConditionalDialogue& A, const FConditionalDialogue& B)
	{
		return A.Priority > B.Priority;
	});

	for (const FConditionalDialogue& Conditional : SortedDialogs)
	{
		if (EvaluateConditions(Player, Conditional.Conditions))
		{
			return Conditional.DialogueData;
		}
	}

	// 조건 충족하는 게 없으면 기본 대화 반환
	return DefaultDialogue;
}

bool UNPCDialogueComponent::CanStartDialogue(ACharacter* Player) const
{
	// 대화 데이터가 아예 없는 NPC
	if (!DefaultDialogue && ConditionalDialogues.IsEmpty())
	{
		return false;
	}

	if (!bAllowMultipleSessions)
	{
		// 이미 누군가 대화 중이면 차단
		return ActiveSessionCount == 0;
	}

	// 최대 인원 초과 여부 확인
	return ActiveSessionCount < MaxMultipleSessionCount;
}

void UNPCDialogueComponent::OnDialogueStarted(ACharacter* Player, ACharacter* NPC)
{
	// 이 NPC의 대화인지 확인
	if (NPC->GetOwner() != GetOwner())
	{
		return;
	}

	ActiveSessionCount++;
}

void UNPCDialogueComponent::OnDialogueEnded(ACharacter* Player)
{
	// 이 NPC와의 대화인지 확인
	UDialogueManagerSubsystem* DialogManager =
		GetWorld()->GetSubsystem<UDialogueManagerSubsystem>();

	if (!DialogManager)
	{
		return;
	}

	ActiveSessionCount = DialogManager->GetActiveSessionCountForNPC(
		Cast<ACharacter>(GetOwner()));
}


bool UNPCDialogueComponent::EvaluateConditions(ACharacter* Player, const TArray<FDialogueCondition>& Conditions) const
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
			// bResult = QuestManager &&
			//		  QuestManager->GetQuestStatus(Condition.TargetQuestID) == Condition.RequiredStatus;
			break;
		case EDialogueConditionType::HasItem:
			// bResult = Player->GetInventory()->HasItem(Condition.RequiredItemID);
			break;
		case EDialogueConditionType::PlayerLevel:
			// bResult = Player->GetLevel() >= Condition.RequiredLevel;
			break;
		case EDialogueConditionType::GameplayTag:
			// bResult = Player->GetAbilitySystemComponent()
			//				->HasMatchingGameplayTag(Condition.RequiredTag);
			break;
		case EDialogueConditionType::DialoguePlayed:
			// bResult = GetWorld()->GetSubsystem<UDialogManagerSubsystem>()
			// 					->HasPlayedDialog(Player, Condition.TargetDialogID);
			break;
		case EDialogueConditionType::RelationshipLevel:
			// bResult = Player->GetRelationshipLevel(Condition.TargetNPCID) >= Condition.RequiredLevel;
			break;
		}

		if (Condition.bNegate)
		{
			bResult = !bResult;
		}
		if (!bResult)
		{
			return false;
		}
	}

	return true;
}
