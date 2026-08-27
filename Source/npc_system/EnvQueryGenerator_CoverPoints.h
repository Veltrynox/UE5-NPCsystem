// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "DataProviders/AIDataProvider.h"
#include "EnvQueryGenerator_CoverPoints.generated.h"

UCLASS(meta = (DisplayName = "Points: Tactical Cover Points"))
class NPC_SYSTEM_API UEnvQueryGenerator_CoverPoints : public UEnvQueryGenerator
{
    GENERATED_BODY()

public:
    UEnvQueryGenerator_CoverPoints();

    virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;
    virtual FText GetDescriptionTitle() const override;
    virtual FText GetDescriptionDetails() const override;

    // Search radius for cover points around the query owner / bot
    UPROPERTY(EditDefaultsOnly, Category = "Generator")
    FAIDataProviderFloatValue SearchRadius;

    // Context for search center (typically Querier)
    UPROPERTY(EditDefaultsOnly, Category = "Generator")
    TSubclassOf<class UEnvQueryContext> SearchCenter;
};
