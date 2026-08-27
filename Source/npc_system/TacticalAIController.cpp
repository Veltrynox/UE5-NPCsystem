#include "TacticalAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"

ATacticalAIController::ATacticalAIController()
{
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 2500.0f;
    SightConfig->LoseSightRadius = 3000.0f;
    SightConfig->PeripheralVisionAngleDegrees = 60.0f;
    SightConfig->SetMaxAge(5.0f);
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
    HearingConfig->HearingRange = 300.0f;
    HearingConfig->SetMaxAge(3.0f);
    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

    AIPerceptionComponent->ConfigureSense(*SightConfig);
    AIPerceptionComponent->ConfigureSense(*HearingConfig);
    AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ATacticalAIController::BeginPlay()
{
    Super::BeginPlay();

    if (AIPerceptionComponent)
    {
        AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ATacticalAIController::OnTargetPerceptionUpdated);
    }
}

void ATacticalAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    UE_LOG(LogTemp, Warning, TEXT("[DEBUG AI] Step 1: Perception triggered! Actor: %s"), *GetNameSafe(Actor));

    UBlackboardComponent* BB = GetBlackboardComponent();
    if (!BB)
    {
        UE_LOG(LogTemp, Error, TEXT("[DEBUG AI] Step 2 FAIL: Blackboard is NULL!"));
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("[DEBUG AI] Step 2 PASS: Blackboard is valid."));

    FAISenseID SightID = UAISense::GetSenseID<UAISense_Sight>();
    FAISenseID HearingID = UAISense::GetSenseID<UAISense_Hearing>();

    UE_LOG(LogTemp, Log, TEXT("[DEBUG AI] Step 3: Stimulus Type = %d (SightID = %d, HearingID = %d)"),
        Stimulus.Type.Index, SightID.Index, HearingID.Index);

    if (Stimulus.Type == SightID)
    {
        bool bSensed = Stimulus.WasSuccessfullySensed();
        UE_LOG(LogTemp, Warning, TEXT("[DEBUG AI] Step 4 (Sight): WasSuccessfullySensed = %d"), bSensed);

        if (bSensed)
        {
            BB->SetValueAsObject(TargetActorKey, Actor);
            BB->SetValueAsVector(LastKnownPosKey, Stimulus.StimulusLocation);
            BB->SetValueAsBool(HasLineOfSightKey, true);
            UE_LOG(LogTemp, Display, TEXT("[DEBUG AI] Step 5: Values written to BB (Key: %s -> %s)"),
                *TargetActorKey.ToString(), *GetNameSafe(Actor));
        }
        else
        {
            BB->SetValueAsVector(LastKnownPosKey, Stimulus.StimulusLocation);
            BB->SetValueAsBool(HasLineOfSightKey, false);
            UE_LOG(LogTemp, Display, TEXT("[DEBUG AI] Step 5: Lost sight. Updated LastKnownPosition."));
        }
    }
    else if (Stimulus.Type == HearingID)
    {
        UE_LOG(LogTemp, Warning, TEXT("[DEBUG AI] Step 4 (Hearing): Sound detected at %s"), *Stimulus.StimulusLocation.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[DEBUG AI] Step 3 FAIL: Stimulus type did not match Sight or Hearing!"));
    }
}
