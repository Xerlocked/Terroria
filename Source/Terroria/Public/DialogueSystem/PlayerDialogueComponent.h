// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueManagerSubsystem.h"
#include "Components/ActorComponent.h"
#include "PlayerDialogueComponent.generated.h"

// 대화 시작 - UI에게 "대화창 열어라" 신호
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLocalDialogStartedDelegate, ACharacter* /* Player */, ACharacter* /* NPC */);

// 노드 변경 - UI에게 "이 내용 보여줘라" 신호
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLocalNodeChangedDelegate, const FDialogueNode& /* Node */);

// 대화 종료 - UI에게 "대화창 닫아라" 신호
DECLARE_MULTICAST_DELEGATE(FOnLocalDialogueEndedDelegate);

UCLASS(ClassGroup=(Dialogue), meta=(BlueprintSpawnableComponent))
class TERRORIA_API UPlayerDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerDialogueComponent();

	FOnLocalDialogStartedDelegate OnLocalDialogueStarted;

	FOnLocalNodeChangedDelegate OnLocalNodeChanged;

	FOnLocalDialogueEndedDelegate OnLocalDialogueEnded;

	UFUNCTION(BlueprintCallable)
	void TryInteract(ACharacter* TargetNPC);

	UFUNCTION(BlueprintCallable)
	void SelectResponse(int32 ResponseIndex);

	UFUNCTION(BlueprintCallable)
	void ExitDialogue();

	UFUNCTION(BlueprintCallable)
	bool IsInDialogue() const { return bIsInDialogue; };

	UFUNCTION(BlueprintCallable)
	ACharacter* GetCurrentNPC() const { return CurrentNPC; }

	UFUNCTION(BlueprintCallable)
	TArray<FDialogueResponse> GetValidResponses() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	UDialogueManagerSubsystem* DialogueManager = nullptr;

	UPROPERTY()
	ACharacter* CurrentNPC = nullptr;

	bool bIsInDialogue = false;

	UFUNCTION()
	void OnDialogueStarted(ACharacter* Player, ACharacter* NPC);

	UFUNCTION()
	void OnNodeChanged(ACharacter* Player, const FDialogueNode& Node);

	UFUNCTION()
	void OnDialogueEnded(ACharacter* Player);

	ACharacter* GetOwnerAsCharacter() const;
};
