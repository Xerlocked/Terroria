// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "TGameplayInput.h"
#include "TGameplayInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TERRORIA_API UTGameplayInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UTGameplayInputComponent();

	template <class T, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UTGameplayInput* InputContext, T* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);

	template <class T, typename AxisFuncType>
	void BindAxisActions(const UTGameplayInput* InputContext, T* Object, AxisFuncType AxisFunc);
};

template <class T, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UTGameplayInputComponent::BindAbilityActions(const UTGameplayInput* InputContext, T* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputContext);

	for (const FGameplayInputAction& Action : InputContext->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTags.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTags);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTags);
			}

			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTags);
			}
		}
	}
}

template <class T, typename AxisFuncType>
void UTGameplayInputComponent::BindAxisActions(const UTGameplayInput* InputContext, T* Object, AxisFuncType AxisFunc)
{
	check(InputContext);

	// for (const FGameplayInputAction& Action : InputContext->AbilityInputActions)
	// {
	// 	if (Action.InputAction && Action.InputTags.IsValid() && Action.bAxisAction)
	// 	{
	// 		if (AxisFunc)
	// 		{
	// 			BindAction(Action.InputAction, ETriggerEvent::Started, Object, AxisFunc, CurrentValue, Action.InputTags);
	// 		}
	// 	}
	// }
}
