// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryGenerator_CoverPoints.h"
#include "TacticalCoverPoint.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "Kismet/GameplayStatics.h"

UEnvQueryGenerator_CoverPoints::UEnvQueryGenerator_CoverPoints()
{
    SearchRadius.DefaultValue = 2500.0f;
    SearchCenter = UEnvQueryContext_Querier::StaticClass();
    ItemType = UEnvQueryItemType_Point::StaticClass();
}

void UEnvQueryGenerator_CoverPoints::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
    UObject* QueryOwner = QueryInstance.Owner.Get();
    if (!QueryOwner) return;

    TArray<FVector> ContextLocations;
    QueryInstance.PrepareContext(SearchCenter, ContextLocations);

    if (ContextLocations.Num() == 0) return;

    SearchRadius.BindData(QueryOwner, QueryInstance.QueryID);
    float RadiusValue = SearchRadius.GetValue();
    float RadiusSq = FMath::Square(RadiusValue);

    FVector CenterLocation = ContextLocations[0];

    // Find all cover actors in the world
    TArray<AActor*> FoundCoverActors;
    UGameplayStatics::GetAllActorsOfClass(QueryOwner->GetWorld(), ATacticalCoverPoint::StaticClass(), FoundCoverActors);

    TArray<FVector> ValidCoverPositions;

    for (AActor* Actor : FoundCoverActors)
    {
        ATacticalCoverPoint* CoverPoint = Cast<ATacticalCoverPoint>(Actor);
        if (CoverPoint && !CoverPoint->bIsOccupied)
        {
            FVector CoverPos = CoverPoint->GetActorLocation();
            if (FVector::DistSquared(CenterLocation, CoverPos) <= RadiusSq)
            {
                ValidCoverPositions.Add(CoverPos);
            }
        }
    }

    // Pass valid cover points to EQS query generator
    QueryInstance.ReserveItemData(ValidCoverPositions.Num());
    for (const FVector& Pos : ValidCoverPositions)
    {
        QueryInstance.AddItemData<UEnvQueryItemType_Point>(Pos);
    }
}

FText UEnvQueryGenerator_CoverPoints::GetDescriptionTitle() const
{
    return FText::FromString(TEXT("Tactical Cover Points"));
}

FText UEnvQueryGenerator_CoverPoints::GetDescriptionDetails() const
{
    return FText::Format(FText::FromString(TEXT("Radius: {0}")), FText::FromString(SearchRadius.ToString()));
}
