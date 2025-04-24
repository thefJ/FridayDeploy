// Fill out your copyright notice in the Description page of Project Settings.

#include "FDGameState.h"
#include "FDTaskActor.h"
#include "Net/UnrealNetwork.h"

AFDGameState::AFDGameState()
{
}

void AFDGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    // Call the Super
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Add properties to replicated for the derived class
    DOREPLIFETIME(AFDGameState, ArtTaskCount);
    DOREPLIFETIME(AFDGameState, DevelopingTaskCount);
    DOREPLIFETIME(AFDGameState, TestingTaskCount);
    DOREPLIFETIME(AFDGameState, BugCount);
    DOREPLIFETIME(AFDGameState, ServerFinishTaskCount);
}

int32 AFDGameState::GetTaskCountByType(ETaskType TaskType) const
{
    switch (TaskType)
    {
    case ETaskType::Art:
        return ArtTaskCount;
    case ETaskType::Developing:
        return DevelopingTaskCount;
    case ETaskType::Testing:
        return TestingTaskCount;
    case ETaskType::Server:
        return ServerFinishTaskCount;
    case ETaskType::Bug:
        return BugCount;
    default:
        return 0;
    }
}

void AFDGameState::ChangeTaskCountByType(ETaskType TaskType, int32 Value)
{
    if (!HasAuthority())
    {
        return;
    }

    switch (TaskType)
    {
    case ETaskType::Art:
        ArtTaskCount += Value;
        return;
    case ETaskType::Developing:
        DevelopingTaskCount += Value;
        return;
    case ETaskType::Testing:
        TestingTaskCount += Value;
        return;
    case ETaskType::Server:
        ServerFinishTaskCount += Value;
        return;
    case ETaskType::Bug:
        BugCount += Value;
        return;
    }
}