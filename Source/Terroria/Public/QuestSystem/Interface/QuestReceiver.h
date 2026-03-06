// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestReceiver.generated.h"

class UQuestReceiverComponent;
struct FGameplayTag;
// This class does not need to be modified.
UINTERFACE()
class UQuestReceiver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERRORIA_API IQuestReceiver
{
	GENERATED_BODY()

public:
	virtual void AddExperience(int32 Amount) = 0;

	virtual void AddCurrency(int32 Amount) = 0;

	virtual void AddItem(FName ItemID, int32 Quantity) = 0;

	virtual void AddGameplayTag(FGameplayTag Tag) = 0;

	virtual UQuestReceiverComponent* GetQuestReceiverComponent() const = 0;
};
