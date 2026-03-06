// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestSystemTypes.h"
#include "Components/ActorComponent.h"
#include "Data/QuestData.h"
#include "QuestReceiverComponent.generated.h"

class UQuestManagerSubsystem;
// 퀘스트 상태 변경 - 퀘스트 로그 UI 갱신
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLocalQuestStatusChangedDelegate, FName /* QuestID */,
                                     EQuestStatus /* NewStatus */);

// 목표 진행도 변경 - 목표 추적 UI 갱신
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnLocalObjectiveUpdatedDelegate, FName /* QuestID */, FName /* ObjectiveID */,
                                       int32 /* CurrentCount */);

// 목표 완료 - 화면에 완료 알림 표시
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLocalObjectiveCompletedDelegate, FName /* QuestID */, FName /* ObjectiveID */);

UCLASS(ClassGroup=(Quest), meta=(BlueprintSpawnableComponent))
class TERRORIA_API UQuestReceiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestReceiverComponent();

	// 퀘스트 수락 / 완료 / 실패 / 포기 시
	FOnLocalQuestStatusChangedDelegate OnLocalQuestStatusChanged;

	// 목표 진행도 변경 시
	FOnLocalObjectiveUpdatedDelegate OnLocalObjectiveUpdated;

	// 목표 완료 시
	FOnLocalObjectiveCompletedDelegate OnLocalObjectiveCompleted;

	// ── 퀘스트 요청 ───────────────────────────────────────

	// 퀘스트 수락 요청
	// 호출 시점 : 대화 이벤트 / QuestGiverComponent::OfferQuest()
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool RequestAcceptQuest(FName QuestID);

	// 퀘스트 포기 요청
	// 호출 시점 : 퀘스트 로그 UI에서 포기 버튼
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool RequestAbandonQuest(FName QuestID);

	// 목표 진행도 갱신 요청
	// 호출 시점 : 몬스터 처치, 아이템 수집 등 게임플레이 이벤트
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void NotifyObjective(FName QuestID, FName ObjectiveID, int32 Count = 1);

	// GameplayTag 기반 목표 일괄 갱신
	// 호출 시점 : 몬스터 사망 이벤트에서 태그로 처리
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void NotifyObjectiveByTag(FGameplayTag TargetTag, int32 Count = 1);

	// ── 추적 퀘스트 ───────────────────────────────────────
	// HUD에 표시할 메인 퀘스트 (플레이어가 직접 지정)

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void SetTrackedQuest(FName QuestID);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void ClearTrackedQuest();

	UFUNCTION(BlueprintCallable, Category = "Quest")
	FName GetTrackedQuestID() const { return TrackedQuestID; }

	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool HasTrackedQuest() const { return TrackedQuestID != NAME_None; }

	// ── 상태 조회 (QuestManager 래핑) ─────────────────────
	// Blueprint에서 QuestManager를 직접 가져올 필요 없이
	// 이 컴포넌트만으로 조회 가능

	UFUNCTION(BlueprintCallable, Category = "Quest")
	EQuestStatus GetQuestStatus(FName QuestID) const;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool IsQuestActive(FName QuestID) const;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool IsQuestCompleted(FName QuestID) const;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool CanAcceptQuest(FName QuestID) const;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	int32 GetObjectiveCount(FName QuestID, FName ObjectiveID) const;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	TArray<FName> GetActiveQuestIDs() const;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	TArray<FName> GetCompletedQuestIDs() const;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	UQuestData* GetQuestData(FName QuestID) const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	// ── 내부 상태 ─────────────────────────────────────────

	UPROPERTY()
	UQuestManagerSubsystem* QuestManager = nullptr;

	// HUD에 표시 중인 추적 퀘스트
	UPROPERTY()
	FName TrackedQuestID = NAME_None;

	UFUNCTION()
	void OnQuestStatusChanged(FName QuestID, EQuestStatus NewStatus);

	UFUNCTION()
	void OnObjectiveUpdated(FName QuestID, FName ObjectiveID, int32 CurrentCount);

	UFUNCTION()
	void OnObjectiveCompleted(FName QuestID, FName ObjectiveID);

	// Helper
	bool EnsureQuestManager() const;
};
