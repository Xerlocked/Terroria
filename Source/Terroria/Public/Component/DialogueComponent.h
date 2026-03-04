// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/DialogueDataAsset.h"
#include "DialogueComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueUpdatedSignature, const FDialogueNode&, CurrentNode);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEndedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestTriggeredSignature, FName, QuestID);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TERRORIA_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDialogueComponent();

	// 외부(보통 앞서 만든 InteractableInterface)에서 호출하여 대화 시작
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue(UDialogueDataAsset* DialogueData);

	// UI에서 플레이어가 버튼을 클릭했을 때 호출할 함수
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SelectOption(const FDialogueOption& SelectedOption);

protected:
	// UI(UMG)에서 이 이벤트들에 바인딩하여 텍스트를 업데이트하거나 창을 닫습니다.
	UPROPERTY(BlueprintAssignable, Category = "Dialogue|Events")
	FOnDialogueUpdatedSignature OnDialogueUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue|Events")
	FOnDialogueEndedSignature OnDialogueEnded;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue|Events")
	FOnQuestTriggeredSignature OnQuestTriggered;

private:
	UPROPERTY()
	TObjectPtr<UDialogueDataAsset> CurrentDialogueData;

	// 현재 재생 중인 대화 노드를 추적하기 위한 캐시 변수
	FName CurrentNodeID;

	void GoToNextNode(FName NodeID);
};
