// Copyright 2025 @xerlock. All Rights Reserved.


#include "Actor/TBaseDropItem.h"

#include "Terroria.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

ATBaseDropItem::ATBaseDropItem()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetUseCCD(true);
	StaticMeshComponent->SetSimulatePhysics(false);
	SetRootComponent(StaticMeshComponent);

	LabelWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("LabelWidgetComponent");
	LabelWidgetComponent->SetupAttachment(GetRootComponent());

	bReplicates = true;
}

void ATBaseDropItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATBaseDropItem, ItemAmount);
}

void ATBaseDropItem::BeginPlay()
{
	Super::BeginPlay();
}

void ATBaseDropItem::SetAmount_Implementation(int32 InAmount)
{
	ItemAmount = InAmount;
}

ETerroriaCursor ATBaseDropItem::GetCursorType()
{
	return ETerroriaCursor::Interact;
}

void ATBaseDropItem::ActiveHighlightActor()
{
	StaticMeshComponent->SetRenderCustomDepth(true);
	StaticMeshComponent->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ATBaseDropItem::DeactiveHighlightActor()
{
	StaticMeshComponent->SetRenderCustomDepth(false);
}

void ATBaseDropItem::OnRep_ItemAmount()
{
	ReceiveItemAmountUpdated(ItemAmount);
}
