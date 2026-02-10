// Copyright 2025 @xerlock. All Rights Reserved.


#include "Actor/TSphereAreaEffector.h"

#include "Components/SphereComponent.h"

ATSphereAreaEffector::ATSphereAreaEffector()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(RootComponent);

	OverlapComponent = SphereComponent;
}
