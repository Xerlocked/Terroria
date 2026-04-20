// Copyright 2025 @xerlock. All Rights Reserved.


#include "QuestSystem/QuestManagerSubsystem.h"

#include "QuestSystem/Interface/QuestReceiver.h"


void UQuestManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("QuestManagerSubsystem 초기화"));
}

void UQuestManagerSubsystem::Deinitialize()
{
	QuestDataMap.Empty();
	QuestStateMap.Empty();
	Super::Deinitialize();
}

void UQuestManagerSubsystem::RegisterQuestData(UQuestData* QuestData)
{
	if (!QuestData || QuestData->QuestName == NAME_None)
	{
		UE_LOG(LogTemp, Warning, TEXT("RegisterQuestData: 유효하지 않은 QuestData"));
		return;
	}

	QuestDataMap.Add(QuestData->QuestName, QuestData);
	UE_LOG(LogTemp, Log, TEXT("퀘스트 등록: %s"), *QuestData->QuestName.ToString());
}

void UQuestManagerSubsystem::RegisterQuestDataArray(const TArray<UQuestData*>& QuestDataArray)
{
	for (UQuestData* Data : QuestDataArray)
	{
		RegisterQuestData(Data);
	}
}

bool UQuestManagerSubsystem::AcceptQuest(FName QuestID)
{
	if (!CanAcceptQuest(QuestID))
	{
		return false;
	}

	const UQuestData* QuestData = QuestDataMap[QuestID];

	FQuestState NewState;
	NewState.Status = EQuestStatus::Active;
	NewState.AcceptedTime = GetWorld()->GetTimeSeconds();

	for (const FQuestObjective& Objective : QuestData->QuestObjectives)
	{
		NewState.ObjectiveCounts.Add(Objective.ObjectiveID, 0);
	}

	QuestStateMap.Add(QuestID, NewState);

	OnQuestStatusChanged.Broadcast(QuestID, EQuestStatus::Active);
	UE_LOG(LogTemp, Log, TEXT("퀘스트 수락: %s"), *QuestID.ToString());
	return true;
}

bool UQuestManagerSubsystem::AbandonQuest(FName QuestID)
{
	FQuestState* State = QuestStateMap.Find(QuestID);
	if (!State)
	{
		return false;
	}

	if (State->Status != EQuestStatus::Active &&
		State->Status != EQuestStatus::ObjectiveComplete)
	{
		return false;
	}

	State->Status = EQuestStatus::Abandoned;

	OnQuestStatusChanged.Broadcast(QuestID, EQuestStatus::Abandoned);
	UE_LOG(LogTemp, Log, TEXT("퀘스트 포기: %s"), *QuestID.ToString());
	return true;
}

bool UQuestManagerSubsystem::CompleteQuest(FName QuestID)
{
	FQuestState* State = QuestStateMap.Find(QuestID);
	if (!State || State->Status == EQuestStatus::Completed)
	{
		return false;
	}

	// Active 또는 ObjectiveComplete 상태에서만 완료 가능
	if (State->Status != EQuestStatus::Active &&
		State->Status != EQuestStatus::ObjectiveComplete)
	{
		UE_LOG(LogTemp, Warning, TEXT("CompleteQuest: %s 는 완료 가능한 상태가 아님 (Status=%d)"),
		       *QuestID.ToString(), static_cast<int32>(State->Status));
		return false;
	}

	// 모든 필수 목표가 완료되었는지 최종 검증
	const UQuestData* QuestData = QuestDataMap.FindRef(QuestID);
	if (QuestData)
	{
		for (const FQuestObjective& Objective : QuestData->QuestObjectives)
		{
			if (!Objective.bIsOptional && !IsObjectiveComplete(QuestID, Objective))
			{
				UE_LOG(LogTemp, Warning,
				       TEXT("CompleteQuest: %s - 필수 목표 %s 가 미완료 상태"),
				       *QuestID.ToString(), *Objective.ObjectiveID.ToString());
				return false;
			}
		}
	}

	State->Status = EQuestStatus::Completed;

	GrantRewards(QuestID);

	OnQuestStatusChanged.Broadcast(QuestID, EQuestStatus::Completed);
	UE_LOG(LogTemp, Log, TEXT("퀘스트 완료: %s"), *QuestID.ToString());
	return true;

	// State->Status = EQuestStatus::Completed;
	//
	// GrantRewards(QuestID);
	//
	// OnQuestStatusChanged.Broadcast(QuestID, EQuestStatus::Completed);
	// UE_LOG(LogTemp, Log, TEXT("퀘스트 완료: %s"), *QuestID.ToString());
	// return true;
}

bool UQuestManagerSubsystem::FailQuest(FName QuestID)
{
	FQuestState* State = QuestStateMap.Find(QuestID);
	if (!State)
	{
		return false;
	}

	if (State->Status != EQuestStatus::Active &&
		State->Status != EQuestStatus::ObjectiveComplete)
	{
		return false;
	}

	State->Status = EQuestStatus::Failed;

	OnQuestStatusChanged.Broadcast(QuestID, EQuestStatus::Failed);
	UE_LOG(LogTemp, Log, TEXT("퀘스트 실패: %s"), *QuestID.ToString());
	return true;
}

void UQuestManagerSubsystem::UpdateObjective(FName QuestID, FName ObjectiveID, int32 Count)
{
	FQuestState* State = QuestStateMap.Find(QuestID);
	if (!State)
	{
		return;
	}

	if (State->Status != EQuestStatus::Active &&
		State->Status != EQuestStatus::ObjectiveComplete)
	{
		return;
	}

	const UQuestData* QuestData = QuestDataMap.FindRef(QuestID);
	if (!QuestData)
	{
		return;
	}

	const FQuestObjective* Objective = QuestData->QuestObjectives.FindByPredicate(
		[&ObjectiveID](const FQuestObjective& Obj)
		{
			return Obj.ObjectiveID == ObjectiveID;
		});

	if (!Objective)
	{
		return;
	}

	// 이미 완료된 목표는 무시
	if (IsObjectiveComplete(QuestID, *Objective))
	{
		return;
	}

	int32& CurrentCount = State->ObjectiveCounts[ObjectiveID];
	CurrentCount = FMath::Clamp(CurrentCount + Count, 0, Objective->RequiredCount);

	OnObjectiveUpdated.Broadcast(QuestID, ObjectiveID, CurrentCount);
	UE_LOG(LogTemp, Log, TEXT("목표 갱신: %s / %s [%d / %d]"),
	       *QuestID.ToString(), *ObjectiveID.ToString(),
	       CurrentCount, Objective->RequiredCount);

	if (IsObjectiveComplete(QuestID, *Objective))
	{
		OnObjectiveCompleted.Broadcast(QuestID, ObjectiveID);
		CheckQuestCompletion(QuestID);
	}
}

void UQuestManagerSubsystem::UpdateObjectiveByTag(FGameplayTag TargetTag, int32 Count)
{
	for (auto& Pair : QuestStateMap)
	{
		const FName& QuestID = Pair.Key;
		const FQuestState& State = Pair.Value;

		if (State.Status != EQuestStatus::Active &&
			State.Status != EQuestStatus::ObjectiveComplete)
		{
			continue;
		}

		const UQuestData* QuestData = QuestDataMap.FindRef(QuestID);
		if (!QuestData)
		{
			continue;
		}

		for (const FQuestObjective& Objective : QuestData->QuestObjectives)
		{
			if (Objective.TargetTag == TargetTag)
			{
				UpdateObjective(QuestID, Objective.ObjectiveID, Count);
			}
		}
	}
}

EQuestStatus UQuestManagerSubsystem::GetQuestStatus(FName QuestID) const
{
	const FQuestState* State = QuestStateMap.Find(QuestID);
	return State ? State->Status : EQuestStatus::NotStarted;
}

bool UQuestManagerSubsystem::IsQuestActive(FName QuestID) const
{
	EQuestStatus Status = GetQuestStatus(QuestID);
	return Status == EQuestStatus::Active ||
		Status == EQuestStatus::ObjectiveComplete;
}

bool UQuestManagerSubsystem::IsQuestCompleted(FName QuestID) const
{
	return GetQuestStatus(QuestID) == EQuestStatus::Completed;
}

bool UQuestManagerSubsystem::CanAcceptQuest(FName QuestID) const
{
	const UQuestData* QuestData = QuestDataMap.FindRef(QuestID);
	if (!QuestData)
	{
		return false;
	}

	EQuestStatus CurrentStatus = GetQuestStatus(QuestID);
	if (CurrentStatus != EQuestStatus::NotStarted &&
		CurrentStatus != EQuestStatus::Abandoned)
	{
		return false;
	}

	return CheckPreRequiredQuests(QuestData);
}

bool UQuestManagerSubsystem::IsQuestPendingPrerequisite(FName QuestID) const
{
	const UQuestData* QuestData = QuestDataMap.FindRef(QuestID);
	if (!QuestData)
	{
		return false;
	}

	const EQuestStatus CurrentStatus = GetQuestStatus(QuestID);
	if (CurrentStatus != EQuestStatus::NotStarted &&
		CurrentStatus != EQuestStatus::Abandoned)
	{
		return false;
	}

	if (QuestData->PreRequiredQuests.IsEmpty())
	{
		return false;
	}

	for (const FName& PrerequisiteID : QuestData->PreRequiredQuests)
	{
		if (!IsQuestCompleted(PrerequisiteID))
		{
			return true;
		}
	}

	return false;
}

int32 UQuestManagerSubsystem::GetObjectiveCount(FName QuestID, FName ObjectiveID) const
{
	const FQuestState* State = QuestStateMap.Find(QuestID);
	if (!State)
	{
		return 0;
	}

	const int32* Count = State->ObjectiveCounts.Find(ObjectiveID);
	return Count ? *Count : 0;
}

TArray<FName> UQuestManagerSubsystem::GetActiveQuestIDs() const
{
	TArray<FName> Result;
	for (const auto& Pair : QuestStateMap)
	{
		if (Pair.Value.Status == EQuestStatus::Active ||
			Pair.Value.Status == EQuestStatus::ObjectiveComplete)
		{
			Result.Add(Pair.Key);
		}
	}
	return Result;
}

TArray<FName> UQuestManagerSubsystem::GetCompletedQuestIDs() const
{
	TArray<FName> Result;
	for (const auto& Pair : QuestStateMap)
	{
		if (Pair.Value.Status == EQuestStatus::Completed)
		{
			Result.Add(Pair.Key);
		}
	}
	return Result;
}

UQuestData* UQuestManagerSubsystem::GetQuestData(FName QuestID) const
{
	UQuestData* const* Found = QuestDataMap.Find(QuestID);
	return Found ? *Found : nullptr;
}

bool UQuestManagerSubsystem::GetQuestState(FName QuestID, FQuestState& OutState) const
{
	const FQuestState* State = QuestStateMap.Find(QuestID);
	if (!State)
	{
		return false;
	}

	OutState = *State;
	return true;
}

void UQuestManagerSubsystem::CheckQuestCompletion(FName QuestID)
{
	const UQuestData* QuestData = QuestDataMap.FindRef(QuestID);
	if (!QuestData)
	{
		return;
	}

	bool bAllRequiredComplete = true;
	bool bAnyComplete = false;

	for (const FQuestObjective& Objective : QuestData->QuestObjectives)
	{
		if (!Objective.bIsOptional && !IsObjectiveComplete(QuestID, Objective))
		{
			bAllRequiredComplete = false;
			break;
		}
	}

	if (!bAllRequiredComplete)
	{
		// 모든 필수 목표가 완료된 게 아니면 상태 변경 없음.
		// Active 상태를 그대로 유지.
		return;
	}
	// // 일부 완료 상태 갱신
	// if (bAnyComplete && !bAllRequiredComplete)
	// {
	// 	FQuestState* State = QuestStateMap.Find(QuestID);
	// 	if (State && State->Status == EQuestStatus::Active)
	// 	{
	// 		State->Status = EQuestStatus::ObjectiveComplete;
	// 		OnQuestStatusChanged.Broadcast(QuestID, EQuestStatus::ObjectiveComplete);
	// 	}
	// }

	// // 전체 완료
	// if (bAllRequiredComplete)
	// {
	// 	if (!QuestData->bRequireReturnToNPC)
	// 	{
	// 		CompleteQuest(QuestID);
	// 	}
	// 	else
	// 	{
	// 		// 위 일부 완료 상태 갱신은 목표가 1개일 경우 검사안하고 바로 완료로 가는 경우 방지
	// 		// NPC에게 돌아가야 하는 퀘스트는 목표 완료 상태로 변경
	// 		FQuestState* State = QuestStateMap.Find(QuestID);
	// 		if (State && State->Status == EQuestStatus::Active)
	// 		{
	// 			State->Status = EQuestStatus::ObjectiveComplete;
	// 			OnQuestStatusChanged.Broadcast(QuestID, EQuestStatus::ObjectiveComplete);
	// 		}
	// 	}
	// }

	// 모든 필수 목표 완료
	if (!QuestData->bRequireReturnToNPC)
	{
		// NPC 보고가 필요 없는 퀘스트는 즉시 완료
		CompleteQuest(QuestID);
	}
	else
	{
		// NPC에게 보고해야 완료되는 퀘스트 → ObjectiveComplete로 전환
		FQuestState* State = QuestStateMap.Find(QuestID);
		if (State && State->Status == EQuestStatus::Active)
		{
			State->Status = EQuestStatus::ObjectiveComplete;
			OnQuestStatusChanged.Broadcast(QuestID, EQuestStatus::ObjectiveComplete);
		}
	}
}

void UQuestManagerSubsystem::GrantRewards(FName QuestID)
{
	const UQuestData* QuestData = QuestDataMap.FindRef(QuestID);
	if (!QuestData)
	{
		return;
	}

	IQuestReceiver* Player = Cast<IQuestReceiver>(GetPlayerPawn());
	if (!Player)
	{
		return;
	}

	for (const FQuestReward& Reward : QuestData->QuestRewards)
	{
		if (Reward.Experience > 0)
		{
			Player->AddExperience(Reward.Experience);
		}

		if (Reward.Currency > 0)
		{
			Player->AddCurrency(Reward.Currency);
		}

		for (const FRewardItem& Item : Reward.Items)
		{
			Player->AddItem(Item.ItemID, Item.Quantity);
		}

		for (const FGameplayTag& Tag : Reward.RewardTags)
		{
			Player->AddGameplayTag(Tag);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("보상 지급: %s"), *QuestID.ToString());
}

bool UQuestManagerSubsystem::CheckPreRequiredQuests(const UQuestData* QuestData) const
{
	for (const FName& PrerequisiteID : QuestData->PreRequiredQuests)
	{
		if (!IsQuestCompleted(PrerequisiteID))
		{
			return false;
		}
	}
	return true;
}

bool UQuestManagerSubsystem::IsObjectiveComplete(FName QuestID, const FQuestObjective& Objective) const
{
	return GetObjectiveCount(QuestID, Objective.ObjectiveID) >= Objective.RequiredCount;
}

APawn* UQuestManagerSubsystem::GetPlayerPawn() const
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC)
	{
		return nullptr;
	}
	return PC->GetPawn();
}
