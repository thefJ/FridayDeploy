#include "FDBugActor.h"

AFDBugActor::AFDBugActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SetActorEnableCollision(false);

    bReplicates = true;
    bAlwaysRelevant = true;
}