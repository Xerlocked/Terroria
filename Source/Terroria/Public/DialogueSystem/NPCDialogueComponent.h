// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogueSystemTypes.h"
#include "Components/ActorComponent.h"
#include "Data/DialogueData.h"
#include "NPCDialogueComponent.generated.h"

class ATPlayerCharacter;

USTRUCT(BlueprintType)
struct FConditionalDialogue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDialogueCondition> Conditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDialogueData* DialogueData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Priority = 0;
};

UCLASS(ClassGroup=(Dialogue), meta=(BlueprintSpawnableComponent))
class TERRORIA_API UNPCDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNPCDialogueComponent();

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	UDialogueData* GetActiveDialogue(ACharacter* Player);

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	bool CanStartDialogue(ACharacter* Player) const;

	UFUNCTION()
	void OnDialogueStarted(ACharacter* Player, ACharacter* NPC);

	UFUNCTION()
	void OnDialogueEnded(ACharacter* Player);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	UDialogueData* DefaultDialogue = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	TArray<FConditionalDialogue> ConditionalDialogues;

	UPROPERTY(BlueprintReadOnly)
	int32 ActiveSessionCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	bool bAllowMultipleSessions = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog", meta = (EditCondition = "bAllowMultipleSessions"))
	int32 MaxMultipleSessionCount = 10;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	bool EvaluateConditions(ACharacter* Player, const TArray<FDialogueCondition>& Conditions) const;
};
