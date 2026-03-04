// Copyright 2025 @xerlock. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Highlight.h"
#include "Interface/Interactable.h"
#include "Interface/Itemable.h"
#include "TBaseDropItem.generated.h"

class UWidgetComponent;

UCLASS()
class TERRORIA_API ATBaseDropItem : public AActor, public IHighlight, public IItemable
{
	GENERATED_BODY()

public:
	ATBaseDropItem();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetAmount_Implementation(int32 InAmount) override;

	virtual ETerroriaCursor GetCursorType() override;
	virtual void ActiveHighlightActor() override;
	virtual void DeactiveHighlightActor() override;

	UFUNCTION()
	void OnRep_ItemAmount();

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveItemAmountUpdated(int32 Amount);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UWidgetComponent> LabelWidgetComponent;

	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_ItemAmount, BlueprintReadWrite, Category = "Property")
	int32 ItemAmount = 0;
};
