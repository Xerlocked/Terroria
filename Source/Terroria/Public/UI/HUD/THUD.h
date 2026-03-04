// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "THUD.generated.h"

class UTShopWidgetController;
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

	UTAttributeMenuWidgetController* GetAttributeMenuWidgetController(
		const FWidgetControllerParams& WidgetControllerParams);

	UTShopWidgetController* GetShopWidgetController(const FWidgetControllerParams& WidgetControllerParams);

	void InitOverlayHUD(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	void InitShopHUD(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	UFUNCTION(BlueprintCallable)
	void ToggleShopWidget() const;

private:
	UPROPERTY()
	TObjectPtr<UTUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UTUserWidget> ShopWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ShopWidgetClass;

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

	/* Shop */
	UPROPERTY()
	TObjectPtr<UTShopWidgetController> ShopWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTShopWidgetController> ShopWidgetControllerClass;

	//~ End Widget Controller
};
