// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "QuestSystemTypes.h"
#include "Data/QuestData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestManagerSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestStatusChanged, FName, QuestID, EQuestStatus, NewStatus);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnObjectiveUpdated, FName, QuestID, FName, ObjectiveID, int32,
                                               CurrentCount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectiveCompleted, FName, QuestID, FName, ObjectiveID);

USTRUCT(BlueprintType)
struct FQuestState
{
	GENERATED_BODY()

	UPROPERTY()
	EQuestStatus Status = EQuestStatus::NotStarted;

	// Key: ObjectiveID / Value: 현재 진행 카운트
	UPROPERTY()
	TMap<FName, int32> ObjectiveCounts;

	UPROPERTY()
	float AcceptedTime = 0.f;
};

/**
 * 
 */
UCLASS()
class TERRORIA_API UQuestManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/******************
	 * 퀘스트 상태 변경 이벤트
	 * 퀘스트 상태가 변경될 때마다 호출됨 (예: 수락, 완료, 실패 등)
	 * UI나 다른 시스템이 퀘스트 상태 변경에 반응할 수 있도록 함
	 ******************
	 */
	UPROPERTY(BlueprintAssignable, Category = "Quest|Events")
	FOnQuestStatusChanged OnQuestStatusChanged;

	UPROPERTY(BlueprintAssignable, Category = "Quest|Events")
	FOnObjectiveUpdated OnObjectiveUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Quest|Events")
	FOnObjectiveCompleted OnObjectiveCompleted;

	/******************
	 * 퀘스트 데이터 등록
	 * 게임 시작 시점에 모든 퀘스트 데이터를 등록해야 함
	 ******************
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void RegisterQuestData(UQuestData* QuestData);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void RegisterQuestDataArray(const TArray<UQuestData*>& QuestDataArray);

	/******************
	 * 퀘스트 상태 변경
	 * 퀘스트 수락, 포기, 완료, 실패 등의 상태 변경을 처리하는 함수들
	 * 각 함수는 퀘스트 ID를 받아 해당 퀘스트의 상태를 변경하고, 필요한 경우 보상 지급이나 후속 퀘스트 활성화 등을 처리함
	 ******************
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool AcceptQuest(FName QuestID);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool AbandonQuest(FName QuestID);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool CompleteQuest(FName QuestID);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool FailQuest(FName QuestID);

	/******************
	 * 목적 업데이트
	 * 특정 퀘스트의 특정 목적에 대한 진행 상황을 업데이트하는 함수
	 * 예: 몬스터 처치, 아이템 수집 등에서 호출되어 해당 목적의 진행 카운트를 증가시키고, 필요 시 퀘스트 완료 여부를 체크
	 ******************
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void UpdateObjective(FName QuestID, FName ObjectiveID, int32 Count = 1);

	// 태그 기반 일괄 업데이트 - 몬스터 처치 등에서 호출
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void UpdateObjectiveByTag(FGameplayTag TargetTag, int32 Count = 1);

	/******************
	 * 상태 조회
	 ******************
	 */
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

	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool GetQuestState(FName QuestID, FQuestState& OutState) const;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	UPROPERTY()
	TMap<FName, UQuestData*> QuestDataMap;

	UPROPERTY()
	TMap<FName, FQuestState> QuestStateMap;

	void CheckQuestCompletion(FName QuestID);
	void GrantRewards(FName QuestID);
	bool CheckPreRequiredQuests(const UQuestData* QuestData) const;
	bool IsObjectiveComplete(FName QuestID, const FQuestObjective& Objective) const;

	APawn* GetPlayerPawn() const;
};
