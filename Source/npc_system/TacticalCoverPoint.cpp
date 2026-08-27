#include "TacticalCoverPoint.h"
#include "Components/ArrowComponent.h"

ATacticalCoverPoint::ATacticalCoverPoint()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    
    CoverDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("CoverDirection"));
    CoverDirection->SetupAttachment(RootComponent);
    CoverDirection->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
    CoverDirection->ArrowColor = FColor::Green;
}

bool ATacticalCoverPoint::IsValidForCover(const FVector& ThreatLocation) const
{
    if (bIsOccupied)
    {
        return false;
    }

    // Vector math: cover forward direction must face towards the threat
    // The angle between cover forward vector and threat direction must be acute (Dot Product > 0)
    FVector DirectionToThreat = (ThreatLocation - GetActorLocation()).GetSafeNormal2D();
    FVector ForwardVector = CoverDirection->GetForwardVector().GetSafeNormal2D();

    float Dot = FVector::DotProduct(ForwardVector, DirectionToThreat);
    return Dot > 0.0f; // Cover protects only when the threat is in front of the wall normal
}
