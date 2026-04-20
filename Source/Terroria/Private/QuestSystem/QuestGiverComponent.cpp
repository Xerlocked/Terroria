// Copyright 2025 @xerlock. All Rights Reserved.


#include "QuestSystem/QuestGiverComponent.h"
#include "QuestSystem/QuestManagerSubsystem.h"


UQuestGiverComponent::UQuestGiverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UQuestGiverComponent::BeginPlay()
{
	Super::BeginPlay();

	QuestManager = GetWorld()->GetGameInstance()->GetSubsystem<UQuestManagerSubsystem>();
	if (!QuestManager)
	{
		UE_LOG(LogTemp, Error, TEXT("QuestGiverComponent: QuestManagerSubsystem 없음"));
		return;
	}

	// OfferedQuests의 데이터를 QuestManager에 자동 등록
	// NPC가 스폰될 때 자신이 가진 퀘스트 데이터를 등록함
	QuestManager->RegisterQuestDataArray(OfferedQuests);

	QuestManager->OnQuestStatusChanged.AddDynamic(this, &UQuestGiverComponent::HandleQuestStatusChanged);
}

void UQuestGiverComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (QuestManager)
	{
		QuestManager->OnQuestStatusChanged.RemoveDynamic(this, &UQuestGiverComponent::HandleQuestStatusChanged);
	}
	Super::EndPlay(EndPlayReason);
}

bool UQuestGiverComponent::EnsureQuestManager() const
{
	if (!QuestManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("QuestGiverComponent: QuestManager 없음 - %s"), *GetOwner()->GetName());
		return false;
	}
	return true;
}

void UQuestGiverComponent::HandleQuestStatusChanged(FName QuestID, EQuestStatus NewStatus)
{
	const bool bIsOwnQuest = OfferedQuests.ContainsByPredicate(
		[&](const UQuestData* Q) { return Q && Q->QuestName == QuestID; });

	const bool bIsPrerequisiteOfOwn = OfferedQuests.ContainsByPredicate(
		[&](const UQuestData* Q)
		{
			return Q && Q->PreRequiredQuests.Contains(QuestID);
		});

	if (!bIsOwnQuest && !bIsPrerequisiteOfOwn)
	{
		return;
	}

	const ENPCMarkerState NewMarker = GetMarkerState();
	if (NewMarker != CachedMarkerState)
	{
		CachedMarkerState = NewMarker;
		OnQuestMarkerStateChanged.Broadcast(NewMarker);
	}
}

TArray<UQuestData*> UQuestGiverComponent::GetAvailableQuests() const
{
	TArray<UQuestData*> Result;
	if (!EnsureQuestManager())
	{
		return Result;
	}

	for (UQuestData* QuestData : OfferedQuests)
	{
		if (!QuestData)
		{
			continue;
		}

		if (QuestManager->CanAcceptQuest(QuestData->QuestName))
		{
			Result.Add(QuestData);
		}
	}

	return Result;
}

TArray<UQuestData*> UQuestGiverComponent::GetCompletableQuests() const
{
	TArray<UQuestData*> Result;
	if (!EnsureQuestManager())
	{
		return Result;
	}

	for (UQuestData* QuestData : OfferedQuests)
	{
		if (!QuestData)
		{
			continue;
		}

		// ObjectiveComplete = 목표는 다 했지만 완료 보고를 아직 안 한 상태
		EQuestStatus Status = QuestManager->GetQuestStatus(QuestData->QuestName);
		if (Status == EQuestStatus::ObjectiveComplete)
		{
			Result.Add(QuestData);
		}
	}

	return Result;
}

TArray<UQuestData*> UQuestGiverComponent::GetActiveQuests() const
{
	TArray<UQuestData*> Result;
	if (!EnsureQuestManager())
	{
		return Result;
	}

	for (UQuestData* QuestData : OfferedQuests)
	{
		if (!QuestData)
		{
			continue;
		}

		if (QuestManager->IsQuestActive(QuestData->QuestName))
		{
			Result.Add(QuestData);
		}
	}

	return Result;
}

bool UQuestGiverComponent::HasPendingChainQuest() const
{
	if (!EnsureQuestManager())
	{
		return false;
	}

	for (const UQuestData* QuestData : OfferedQuests)
	{
		if (!QuestData)
		{
			continue;
		}

		if (QuestManager->IsQuestPendingPrerequisite(QuestData->QuestName))
		{
			return true;
		}
	}

	return false;
}

bool UQuestGiverComponent::HasAvailableQuest() const
{
	return GetAvailableQuests().Num() > 0;
}

ENPCMarkerState UQuestGiverComponent::GetMarkerState() const
{
	if (!EnsureQuestManager())
	{
		return ENPCMarkerState::None;
	}

	// 완료 보고 가능한 퀘스트가 있으면 최우선
	if (GetCompletableQuests().Num() > 0)
	{
		return ENPCMarkerState::QuestComplete;
	}

	// 수락 가능한 퀘스트가 있으면 두 번째
	if (GetAvailableQuests().Num() > 0)
	{
		return ENPCMarkerState::QuestAvail;
	}

	// 진행 중인 퀘스트가 있으면 세 번째
	if (GetActiveQuests().Num() > 0)
	{
		return ENPCMarkerState::QuestActive;
	}

	return ENPCMarkerState::None;
}

bool UQuestGiverComponent::OfferQuest(FName QuestID)
{
	if (!EnsureQuestManager())
	{
		return false;
	}

	// 이 NPC가 제공하는 퀘스트인지 확인
	const bool bOwnsQuest = OfferedQuests.ContainsByPredicate(
		[&QuestID](const UQuestData* QuestData)
		{
			return QuestData && QuestData->QuestName == QuestID;
		});

	if (!bOwnsQuest)
	{
		UE_LOG(LogTemp, Warning, TEXT("OfferQuest: %s 는 이 NPC의 퀘스트가 아님 - %s"),
		       *QuestID.ToString(), *GetOwner()->GetName());
		return false;
	}

	return QuestManager->AcceptQuest(QuestID);
}
