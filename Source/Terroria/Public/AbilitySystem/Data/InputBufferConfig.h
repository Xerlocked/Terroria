// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputBufferConfig.generated.h"

USTRUCT(BlueprintType)
struct FInputBufferEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Priority = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ValidityDuration = 0.5f;
};

UCLASS(BlueprintType)
class TERRORIA_API UInputBufferConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FInputBufferEntry> InputBufferEntries;

	bool FindEntryByTag(const FGameplayTag& InputTag, FInputBufferEntry& OutEntry) const
	{
		for (const FInputBufferEntry& Entry : InputBufferEntries)
		{
			if (Entry.InputTag.MatchesTagExact(InputTag))
			{
				OutEntry = Entry;
				return true;
			}
		}
		return false;
	}
};
