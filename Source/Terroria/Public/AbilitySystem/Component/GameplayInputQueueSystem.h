// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "GameplayInputQueueSystem.generated.h"


class UInputBufferConfig;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FBufferedInput
{
	GENERATED_BODY()

	FBufferedInput() = default;

	/** 입력 식별 태그 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FGameplayTag InputTag = FGameplayTag();

	/** 입력 우선 순위, 값이 높을수록 우선 처리 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	int32 Priority;

	/** 입력 버퍼 내에서 유효한 지속 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float ValidityDuration;

	/** 입력이 발생한 월드 시간 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	double TimeStamp;

	FBufferedInput(const FGameplayTag InInputTag, const double InTimeStamp, const int32 InPriority = 0,
	               const float InValidityDuration = 0.5f) :
		InputTag(InInputTag), Priority(InPriority), ValidityDuration(InValidityDuration), TimeStamp(InTimeStamp)
	{
	}

	bool IsValid(const double CurrentTime) const
	{
		return CurrentTime - TimeStamp <= ValidityDuration;
	}

	bool operator<(const FBufferedInput& Other) const
	{
		if (Priority != Other.Priority)
		{
			return Priority < Other.Priority;
		}
		return TimeStamp < Other.TimeStamp;
	}

	FString ToString() const
	{
		return FString::Printf(TEXT("[%s] Time: %.2f, Dur: %.2f, Pri: %d"),
		                       *InputTag.ToString(), TimeStamp, ValidityDuration, Priority);
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputQueueConsumedDelegate, FGameplayTag, ConsumedInputTag);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TERRORIA_API UGameplayInputQueueSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UGameplayInputQueueSystem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Input")
	bool AddInputToBuffer(const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void ClearBuffer();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void BeginBufferWindow(const FGameplayTagContainer& AllowedTags);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void EndBufferWindow();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetInputBufferEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Input")
	bool IsInputBufferEnabled() const { return bIsBufferEnabled; }

protected:
	/**
	 * GAS를 통해 단일 입력 실행을 시도합니다.
	 * @param InputItem 버퍼링된 단일 입력 아이템
	 * @return 성공 시 true
	 */
	bool TryActivateBufferedAbility(const FBufferedInput& InputItem);

	/** 시간이 지난 입력들을 제거합니다. */
	void PurgeExpiredInputs();

	/** 디버깅 정보 표시 */
	void ShowDebug();

public:
	UPROPERTY(BlueprintAssignable, Category = "QueueSystem|Input")
	FInputQueueConsumedDelegate OnInputConsumed;

private:
	TArray<FBufferedInput> InputQueue;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	FGameplayTagContainer AllowedInputTags;

	UPROPERTY(EditDefaultsOnly, Category = "QueueSystem")
	TObjectPtr<UInputBufferConfig> InputBufferConfig;

	UPROPERTY(EditDefaultsOnly, Category = "QueueSystem")
	bool bIsBufferEnabled;

	UPROPERTY(EditAnywhere, Category = "QueueSystem|Debug")
	bool bShowDebug;
};
