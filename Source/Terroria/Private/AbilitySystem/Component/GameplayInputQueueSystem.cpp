// Copyright 2025 @xerlock. All Rights Reserved.


#include "AbilitySystem/Component/GameplayInputQueueSystem.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/Data/InputBufferConfig.h"


UGameplayInputQueueSystem::UGameplayInputQueueSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	bIsBufferEnabled = false;
	bShowDebug = false;
}

void UGameplayInputQueueSystem::BeginPlay()
{
	Super::BeginPlay();

	// Caching
	if (AActor* Owner = GetOwner())
	{
		AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner);

		// Controller인가?
		if (!AbilitySystemComponent)
		{
			APlayerController* PC = Cast<APlayerController>(Owner);
			if (PC)
			{
				AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PC->GetPawn());
			}
		}
	}

	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner does not have an AbilitySystemComponent!"));
	}
}

void UGameplayInputQueueSystem::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsBufferEnabled || !AbilitySystemComponent || InputQueue.Num() == 0)
	{
		return;
	}

	PurgeExpiredInputs();
	if (InputQueue.Num() == 0)
	{
		return;
	}

	// 3. 처리할 입력 후보 선정 (Selection Phase)
	// 가장 최근에 들어온 입력을 우선 처리하거나(LIFO), 우선순위대로 정렬할 수 있습니다.
	// 격투 게임이나 액션 RPG는 보통 가장 '최근'의 입력을 선호합니다.
	// 따라서 배열의 마지막 요소를 확인합니다.
	int32 CandidateIndex = InputQueue.Num() - 1;
	const FBufferedInput& CandidateInput = InputQueue[CandidateIndex];

	if (TryActivateBufferedAbility(CandidateInput))
	{
		// 성공했다면:
		// A. 이벤트 방송
		OnInputConsumed.Broadcast(CandidateInput.InputTag);
		InputQueue.RemoveAt(CandidateIndex);

		if (bShowDebug)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan,
			                                 FString::Printf(
				                                 TEXT("Executed: %s"), *CandidateInput.InputTag.ToString()));
		}
	}
}

bool UGameplayInputQueueSystem::AddInputToBuffer(const FGameplayTag& InputTag)
{
	if (!bIsBufferEnabled)
	{
		return false;
	}

	if (AllowedInputTags.Num() > 0 && !AllowedInputTags.HasTagExact(InputTag))
	{
		if (bShowDebug)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
			                                 FString::Printf(TEXT("Rejected (Not Allowed): %s"), *InputTag.ToString()));
		}
		return false;
	}

	int32 Priority = 0;
	float Duration = 0.5f;

	if (InputBufferConfig)
	{
		FInputBufferEntry Entry;
		if (InputBufferConfig->FindEntryByTag(InputTag, Entry))
		{
			Priority = Entry.Priority;
			Duration = Entry.ValidityDuration;
		}
	}

	const double CurrentTime = GetWorld()->GetTimeSeconds();
	const FBufferedInput NewInput(InputTag, CurrentTime, Duration, Priority);
	InputQueue.Add(NewInput);

	if (bShowDebug)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
		                                 FString::Printf(TEXT("Buffered: %s"), *InputTag.ToString()));
	}

	return true;
}

void UGameplayInputQueueSystem::ClearBuffer()
{
	InputQueue.Empty();
}

void UGameplayInputQueueSystem::BeginBufferWindow(const FGameplayTagContainer& AllowedTags)
{
	AllowedInputTags = AllowedTags;
	bIsBufferEnabled = true;
}

void UGameplayInputQueueSystem::EndBufferWindow()
{
	bIsBufferEnabled = false;
	AllowedInputTags.Reset();
	ClearBuffer();
}

void UGameplayInputQueueSystem::SetInputBufferEnabled(bool bEnabled)
{
	bIsBufferEnabled = bEnabled;
}

bool UGameplayInputQueueSystem::TryActivateBufferedAbility(const FBufferedInput& InputItem)
{
	if (!AbilitySystemComponent)
	{
		return false;
	}

	for (FGameplayAbilitySpec& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputItem.InputTag))
		{
			if (!AbilitySpec.IsActive())
			{
				return AbilitySystemComponent->TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}

	return false;
}

void UGameplayInputQueueSystem::PurgeExpiredInputs()
{
	double CurrentTime = GetWorld()->GetTimeSeconds();

	// RemoveAll을 사용하여 조건에 맞는 요소를 효율적으로 제거합니다.
	InputQueue.RemoveAll([CurrentTime](const FBufferedInput& Item)
	{
		return !Item.IsValid(CurrentTime);
	});
}

void UGameplayInputQueueSystem::ShowDebug()
{
	// show debug info
	UE_LOG(LogTemp, Warning, TEXT("Show Debug"));
}
