// Copyright 2025 @xerlock. All Rights Reserved.


#include "Input/TGameplayInput.h"

const UInputAction* UTGameplayInput::FindInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound)
{
	for (const FGameplayInputAction& InputAction : AbilityInputActions)
	{
		if (InputAction.InputAction && InputAction.InputTags == InputTag)
		{
			return InputAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Found InputAction: %s"), *InputTag.GetTagName().ToString());
	}

	return nullptr;
}
