// Copyright 2025 @xerlock. All Rights Reserved.


#include "QuestSystem/QuestReceiverComponent.h"

#include "QuestSystem/QuestManagerSubsystem.h"

UQuestReceiverComponent::UQuestReceiverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UQuestReceiverComponent::BeginPlay()
{
	Super::BeginPlay();

	QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UQuestManagerSubsystem>();

	if (!EnsureQuestManager())
	{
		return;
	}

	QuestManager->OnQuestStatusChanged.AddDynamic(this, &UQuestReceiverComponent::OnQuestStatusChanged);
	QuestManager->OnObjectiveUpdated.AddDynamic(this, &UQuestReceiverComponent::OnObjectiveUpdated);
	QuestManager->OnObjectiveCompleted.AddDynamic(this, &UQuestReceiverComponent::OnObjectiveCompleted);
}

void UQuestReceiverComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (QuestManager)
	{
		QuestManager->OnQuestStatusChanged.RemoveDynamic(this, &UQuestReceiverComponent::OnQuestStatusChanged);
		QuestManager->OnObjectiveUpdated.RemoveDynamic(this, &UQuestReceiverComponent::OnObjectiveUpdated);
		QuestManager->OnObjectiveCompleted.RemoveDynamic(this, &UQuestReceiverComponent::OnObjectiveCompleted);
	}

	Super::EndPlay(EndPlayReason);
}

bool UQuestReceiverComponent::RequestAcceptQuest(FName QuestID)
{
	if (!EnsureQuestManager())
	{
		return false;
	}

	return QuestManager->AcceptQuest(QuestID);
}

bool UQuestReceiverComponent::RequestAbandonQuest(FName QuestID)
{
	if (!EnsureQuestManager())
	{
		return false;
	}

	// 추적 중인 퀘스트를 포기하면 추적 해제
	if (TrackedQuestID == QuestID)
	{
		ClearTrackedQuest();
	}

	return QuestManager->AbandonQuest(QuestID);
}

void UQuestReceiverComponent::NotifyObjective(FName QuestID, FName ObjectiveID, int32 Count)
{
	if (!EnsureQuestManager())
	{
		return;
	}

	QuestManager->UpdateObjective(QuestID, ObjectiveID, Count);
}

void UQuestReceiverComponent::NotifyObjectiveByTag(FGameplayTag TargetTag, int32 Count)
{
	if (!EnsureQuestManager())
	{
		return;
	}

	QuestManager->UpdateObjectiveByTag(TargetTag, Count);
}

void UQuestReceiverComponent::SetTrackedQuest(FName QuestID)
{
	if (!EnsureQuestManager())
	{
		return;
	}

	// 활성 퀘스트만 추적 가능
	if (!QuestManager->IsQuestActive(QuestID))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetTrackedQuest: %s 는 활성 퀘스트가 아님"),
		       *QuestID.ToString());
		return;
	}

	TrackedQuestID = QuestID;

	UE_LOG(LogTemp, Log, TEXT("추적 퀘스트 설정: %s"), *QuestID.ToString());
}

void UQuestReceiverComponent::ClearTrackedQuest()
{
	TrackedQuestID = NAME_None;
}

EQuestStatus UQuestReceiverComponent::GetQuestStatus(FName QuestID) const
{
	if (!EnsureQuestManager())
	{
		return EQuestStatus::NotStarted;
	}
	return QuestManager->GetQuestStatus(QuestID);
}

bool UQuestReceiverComponent::IsQuestActive(FName QuestID) const
{
	if (!EnsureQuestManager())
	{
		return false;
	}
	return QuestManager->IsQuestActive(QuestID);
}

bool UQuestReceiverComponent::IsQuestCompleted(FName QuestID) const
{
	if (!EnsureQuestManager())
	{
		return false;
	}
	return QuestManager->IsQuestCompleted(QuestID);
}

bool UQuestReceiverComponent::CanAcceptQuest(FName QuestID) const
{
	if (!EnsureQuestManager())
	{
		return false;
	}
	return QuestManager->CanAcceptQuest(QuestID);
}

int32 UQuestReceiverComponent::GetObjectiveCount(FName QuestID, FName ObjectiveID) const
{
	if (!EnsureQuestManager())
	{
		return 0;
	}
	return QuestManager->GetObjectiveCount(QuestID, ObjectiveID);
}

TArray<FName> UQuestReceiverComponent::GetActiveQuestIDs() const
{
	if (!EnsureQuestManager())
	{
		return {};
	}
	return QuestManager->GetActiveQuestIDs();
}

TArray<FName> UQuestReceiverComponent::GetCompletedQuestIDs() const
{
	if (!EnsureQuestManager())
	{
		return {};
	}
	return QuestManager->GetCompletedQuestIDs();
}

UQuestData* UQuestReceiverComponent::GetQuestData(FName QuestID) const
{
	if (!EnsureQuestManager())
	{
		return nullptr;
	}
	return QuestManager->GetQuestData(QuestID);
}

void UQuestReceiverComponent::OnQuestStatusChanged(FName QuestID, EQuestStatus NewStatus)
{
	// 추적 중인 퀘스트가 완료 / 실패 / 포기되면 추적 해제
	if (TrackedQuestID == QuestID)
	{
		if (NewStatus == EQuestStatus::Completed ||
			NewStatus == EQuestStatus::Failed ||
			NewStatus == EQuestStatus::Abandoned)
		{
			ClearTrackedQuest();
		}
	}

	// UI로 전파
	OnLocalQuestStatusChanged.Broadcast(QuestID, NewStatus);
}

void UQuestReceiverComponent::OnObjectiveUpdated(FName QuestID, FName ObjectiveID, int32 CurrentCount)
{
	OnLocalObjectiveUpdated.Broadcast(QuestID, ObjectiveID, CurrentCount);
}

void UQuestReceiverComponent::OnObjectiveCompleted(FName QuestID, FName ObjectiveID)
{
	OnLocalObjectiveCompleted.Broadcast(QuestID, ObjectiveID);
}

bool UQuestReceiverComponent::EnsureQuestManager() const
{
	if (!QuestManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("UQuestComponent: QuestManager 없음"));
		return false;
	}
	return true;
}
