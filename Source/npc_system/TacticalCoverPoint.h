#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TacticalCoverPoint.generated.h"

UENUM(BlueprintType)
enum class ECoverType : uint8
{
    FullCover     UMETA(DisplayName = "Full Cover (Stand)"),
    HalfCover     UMETA(DisplayName = "Half Cover (Crouch)")
};

UCLASS()
class NPC_SYSTEM_API ATacticalCoverPoint : public AActor
{
    GENERATED_BODY()

public:
    ATacticalCoverPoint();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cover Setup")
    ECoverType CoverType = ECoverType::FullCover;

    // Direction the cover is facing (back against the wall, facing the threat)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cover Setup")
    TObjectPtr<class UArrowComponent> CoverDirection;

    // Flag indicating whether this cover is currently occupied by another agent
    UPROPERTY(Transient, BlueprintReadWrite, Category = "Cover State")
    bool bIsOccupied = false;

    UPROPERTY(Transient, BlueprintReadWrite, Category = "Cover State")
    TWeakObjectPtr<AActor> OccupyingActor = nullptr;

    bool IsValidForCover(const FVector& ThreatLocation) const;
};
