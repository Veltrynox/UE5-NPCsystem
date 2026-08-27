#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "TacticalAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

UCLASS()
class NPC_SYSTEM_API ATacticalAIController : public AAIController
{
    GENERATED_BODY()

public:
    ATacticalAIController();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI | Perception")
    TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI | Perception")
    TObjectPtr<UAISenseConfig_Sight> SightConfig;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI | Perception")
    TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    UPROPERTY(EditDefaultsOnly, Category = "AI | Blackboard")
    FName TargetActorKey = "TargetActor";

    UPROPERTY(EditDefaultsOnly, Category = "AI | Blackboard")
    FName LastKnownPosKey = "LastKnownPosition";

    UPROPERTY(EditDefaultsOnly, Category = "AI | Blackboard")
    FName HasLineOfSightKey = "HasLineOfSight";
};
