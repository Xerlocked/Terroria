// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "THUD.generated.h"

class UTAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UTOverlayWidgetController;
class UTUserWidget;

/**
 * 
 */
UCLASS()
class TERRORIA_API ATHUD : public AHUD
{
	GENERATED_BODY()

public:
	UTOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);

	UTAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	
	void InitOverlayHUD(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:

	UPROPERTY()
	TObjectPtr<UTUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;

	//~ Begin Widget Controller

	/* Overlay */
	UPROPERTY()
	TObjectPtr<UTOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTOverlayWidgetController> OverlayWidgetControllerClass;

	/* Attribute Menu */
	UPROPERTY()
	TObjectPtr<UTAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
	
	//~ End Widget Controller
};
