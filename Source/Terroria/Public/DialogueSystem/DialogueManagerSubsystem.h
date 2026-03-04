// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/DialogueNode.h"
#include "Subsystems/WorldSubsystem.h"
#include "DialogueSystem/Data/DialogueNode.h"
#include "DialogueManagerSubsystem.generated.h"

class ACharacter;
class ATNonPlayerCharacter;
class UDialogueData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogueStarted, ACharacter*, Player, ACharacter*, NPC);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNodeChanged, ACharacter*, Player, const FDialogueNode&, Node);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueEnded, ACharacter*, Player);


USTRUCT()
struct FDialogueSession
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UDialogueData> DialogueData = nullptr;

	UPROPERTY()
	TObjectPtr<ACharacter> NPC = nullptr;

	UPROPERTY()
	TObjectPtr<ACharacter> Player = nullptr;

	UPROPERTY()
	FName CurrentNodeID = NAME_None;

	bool IsValid() const
	{
		return DialogueData != nullptr
			&& NPC != nullptr
			&& Player != nullptr
			&& CurrentNodeID != NAME_None;
	}
};

/**
 * 
 */
UCLASS()
class TERRORIA_API UDialogueManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnDialogueStarted OnDialogueStarted;

	UPROPERTY(BlueprintAssignable)
	FOnNodeChanged OnNodeChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDialogueEnded OnDialogueEnded;

	UFUNCTION(BlueprintCallable)
	bool StartDialogue(ACharacter* Player, ACharacter* NPC, UDialogueData* DialogueData);

	UFUNCTION(BlueprintCallable)
	void SelectResponse(ACharacter* Player, int32 Index);

	UFUNCTION(BlueprintCallable)
	void EndDialogue(ACharacter* Player);

	UFUNCTION(BlueprintCallable)
	bool GetCurrentNode(ACharacter* Player, FDialogueNode& OutNode) const;

	const FDialogueNode* GetCurrentNodeInternal(ACharacter* Player) const;

	UFUNCTION(BlueprintCallable)
	bool IsInDialogue(ACharacter* Player) const;

	UFUNCTION(BlueprintCallable)
	int32 GetActiveSessionCountForNPC(ACharacter* NPC) const;

	bool EvaluateConditions(ACharacter* Player, const TArray<FDialogueCondition>& Conditions);

private:
	UPROPERTY()
	TMap<ACharacter*, FDialogueSession> ActiveSessions;

	void ProcessEvents(ACharacter* Player, const TArray<FDialogueEvent>& Events);

	const FDialogueNode* FindNode(const UDialogueData* DialogueData, FName NodeID) const;

	void NextToNode(ACharacter* Player, FName NextNodeID);

	FDialogueSession* GetSession(const ACharacter* Player);
	const FDialogueSession* GetSession(const ACharacter* Player) const;
};
