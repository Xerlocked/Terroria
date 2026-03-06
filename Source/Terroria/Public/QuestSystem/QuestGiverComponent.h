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

UCLASS(ClassGroup=(Quest), meta=(BlueprintSpawnableComponent))
class TERRORIA_API UQuestGiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestGiverComponent();

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

	// 퀘스트 제공 가능 여부
	UFUNCTION(BlueprintCallable, Category = "QuestGiver")
	bool HasAvailableQuest() const;

	// 머리 위 마커 상태 반환
	// 우선순위 : QuestComplete > QuestAvail > QuestActive > None
	UFUNCTION(BlueprintCallable, Category = "QuestGiver")
	ENPCMarkerState GetMarkerState() const;

	UFUNCTION(BlueprintCallable, Category = "QuestGiver")
	bool OfferQuest(FName QuestID);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UQuestManagerSubsystem* QuestManager = nullptr;

	bool EnsureQuestManager() const;
};
