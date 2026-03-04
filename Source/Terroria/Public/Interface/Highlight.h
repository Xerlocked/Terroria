// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Terroria.h"
#include "UObject/Interface.h"
#include "Highlight.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHighlight : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERRORIA_API IHighlight
{
	GENERATED_BODY()

public:
	virtual ETerroriaCursor GetCursorType() = 0;

	virtual void ActiveHighlightActor() = 0;
	virtual void DeactiveHighlightActor() = 0;
};
