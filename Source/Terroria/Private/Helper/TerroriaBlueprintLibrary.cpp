// Copyright 2025 @xerlock. All Rights Reserved.


#include "Helper/TerroriaBlueprintLibrary.h"

#include "GameplayTagContainer.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Struct.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/HUD/THUD.h"

FVector UTerroriaBlueprintLibrary::GetFurthestValidLocation(
	const UObject* WorldContextObject,
	const FVector& StartLocation, const FVector& EndLocation,
	const UCapsuleComponent* CapsuleComponent,
	const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
	const TArray<AActor*>& ActorsToIgnore,
	EDrawDebugTrace::Type DrawDebugType,
	bool bIgnoreSelf)
{
	FVector OutVector = StartLocation;
	UWorld* World = WorldContextObject->GetWorld();

	FVector UnitDirection = (EndLocation - StartLocation).GetSafeNormal();
	const float CapsuleDiameter = CapsuleComponent->GetScaledCapsuleRadius() * 2.f;

	const float Dist = FVector::Dist(StartLocation, EndLocation);
	const int32 CapsuleSteps = FMath::FloorToInt32(Dist / CapsuleDiameter);

	for (int32 i = 0; i < CapsuleSteps; i++)
	{
		// const int32 Step = CapsuleSteps - i;
		// const FVector Location = UnitDirection * Step * CapsuleDiameter + StartLocation;
		//
		// // 캡슐이 이동 가능한 곳에 뭐가 있는지
		// TArray<FHitResult> HitResults;
		// UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		// 	World,
		// 	Location,
		// 	Location,
		// 	CapsuleComponent->GetUnscaledCapsuleRadius(),
		// 	CapsuleComponent->GetUnscaledCapsuleHalfHeight(),
		// 	ObjectTypes,
		// 	false,
		// 	ActorsToIgnore,
		// 	DrawDebugType,
		// 	HitResults,
		// 	bIgnoreSelf
		// );
		//
		// if (IsValidLocation(WorldContextObject, Location, HitResults))
		// {
		// 	OutVector = Location;
		// 	break;
		// }

		const int32 Step = CapsuleSteps - i;
		const FVector BaseLocation = UnitDirection * Step * CapsuleDiameter + StartLocation;

		FVector AdjustedLocation = BaseLocation;

		// 네비게이션 시스템을 이용해 Z축 높이 보정 및 절벽 방지
		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(World);
		FNavLocation ProjectedLocation;

		// 위아래 경사로의 NavMesh를 찾음
		FVector QueryExtent(CapsuleComponent->GetScaledCapsuleRadius(), CapsuleComponent->GetScaledCapsuleRadius(),
		                    500.0f);

		if (NavSys && NavSys->ProjectPointToNavigation(BaseLocation, ProjectedLocation, QueryExtent))
		{
			// 캡슐 높이의 보정
			AdjustedLocation = ProjectedLocation.Location;
			AdjustedLocation.Z += CapsuleComponent->GetScaledCapsuleHalfHeight();

			TArray<FHitResult> HitResults;
			UKismetSystemLibrary::CapsuleTraceMultiForObjects(
				World,
				AdjustedLocation,
				AdjustedLocation,
				CapsuleComponent->GetUnscaledCapsuleRadius(),
				CapsuleComponent->GetUnscaledCapsuleHalfHeight(),
				ObjectTypes,
				false,
				ActorsToIgnore,
				DrawDebugType,
				HitResults,
				bIgnoreSelf
			);

			if (IsValidLocation(WorldContextObject, AdjustedLocation, HitResults))
			{
				OutVector = AdjustedLocation;
				break;
			}
		}
	}

	return OutVector;
}

ATHUD* UTerroriaBlueprintLibrary::GetTHUD(const UObject* WorldContextObject)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (PC)
	{
		return Cast<ATHUD>(PC->GetHUD());
	}

	return nullptr;
}

bool UTerroriaBlueprintLibrary::IsValidLocation(const UObject* WorldContextObject, const FVector& Location,
                                                const TArray<FHitResult>& HitResults)
{
	// 땅인가?
	FHitResult HitResult;
	bool bBlocking = WorldContextObject->GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Location,
		Location + FVector(0.f, 0.f, -500.f),
		ECC_Visibility
	);

	if (!bBlocking)
	{
		return false;
	}

	if (HitResults.IsEmpty())
	{
		return true;
	}

	for (const FHitResult& Hit : HitResults)
	{
		if (Hit.GetComponent()->GetCollisionResponseToChannel(ECC_Pawn) == ECR_Block)
		{
			return false;
		}
	}

	return true;
}
