// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/QuestData.h"
#include "QuestGiverComponent.generated.h"

class UQuestManagerSubsystem;

UENUM(BlueprintType)
enum class ENPCMarkerState : uint8
{
	None UMETA(DisplayName = "None", ToolTip = "표시할 마커 없음"),
	QuestAvail UMETA(DisplayName = "QuestAvail", ToolTip = "수락 가능한 퀘스트 있음"),
	QuestActive UMETA(DisplayName = "QuestActive", ToolTip = "진행 중인 퀘스트 있음"),
	QuestComplete UMETA(DisplayName = "QuestComplete", ToolTip = "완료 가능한 퀘스트 있음"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestMarkerStateChanged, ENPCMarkerState, NewState);

UCLASS(ClassGroup=(Quest), meta=(BlueprintSpawnableComponent))
class TERRORIA_API UQuestGiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestGiverComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnQuestMarkerStateChanged OnQuestMarkerStateChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestGiverComponent")
	TArray<UQuestData*> OfferedQuests;

	// 현재 플레이어가 수락 가능한 퀘스트 목록 반환
	// 호출 시점 : 대화 시작 시, 마커 상태 갱신 시
	UFUNCTION(BlueprintCallable, Category = "QuestGiver")
	TArray<UQuestData*> GetAvailableQuests() const;

	// 현재 플레이어가 완료 보고 가능한 퀘스트 목록 반환
	// 완료 보고가 필요한 퀘스트 (NPC에게 직접 돌아와야 하는 경우)
	UFUNCTION(BlueprintCallable, Category = "QuestGiver")
	TArray<UQuestData*> GetCompletableQuests() const;

	// 진행 중인 퀘스트 목록 반환
	UFUNCTION(BlueprintCallable, Category = "QuestGiver")
	TArray<UQuestData*> GetActiveQuests() const;

	/** 
	 * 이 NPC가 제공하는 퀘스트 중, 선행 퀘스트만 끝나면 바로 받을 수 있는 것들.
	 * 연계 퀘스트가 있는 NPC라도 마커를 진행 중(QuestActive)으로만 
	 * 표시하지 않도록 하기 위한 헬퍼.
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestGiver")
	bool HasPendingChainQuest() const;

	// 퀘스트 제공 가능 여부
	UFUNCTION(BlueprintCallable, Category = "QuestGiver")
	bool HasAvailableQuest() const;

	// 머리 위 마커 상태 반환
	// 우선순위 : QuestComplete > QuestAvail > QuestActive > None
	UFUNCTION(BlueprintCallable, Category = "QuestGiver")
	ENPCMarkerState GetMarkerState() const;

	UFUNCTION(BlueprintCallable, Category = "QuestGiver")
	bool OfferQuest(FName QuestID);

private:
	UPROPERTY()
	UQuestManagerSubsystem* QuestManager = nullptr;

	bool EnsureQuestManager() const;

	UFUNCTION()
	void HandleQuestStatusChanged(FName QuestID, EQuestStatus NewStatus);

	ENPCMarkerState CachedMarkerState = ENPCMarkerState::None;
};
