// Fill out your copyright notice in the Description page of Project Settings.

#include "FDGameState.h"
#include "FDTaskActor.h"
#include "FDGameMode.h"
#include "FDComputerActor.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

AFDGameState::AFDGameState()
{
}

void AFDGameState::BeginPlay()
{
    Super::BeginPlay();

    RemainingTime = MaxRemainTime;
}

void AFDGameState::HandleMatchHasStarted()
{
    Super::HandleMatchHasStarted();

    if (GetLocalRole() == ROLE_Authority)
    {
        // Запускаем таймер (обновление каждую секунду)
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle,
            this,
            &AFDGameState::UpdateGameTimer,
            1.0f,
            true);
    }
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
    DOREPLIFETIME(AFDGameState, RemainingTime);
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
        OnArtTaskCount();
        return;
    case ETaskType::Developing:
        DevelopingTaskCount += Value;
        OnDevelopingTaskCount();
        return;
    case ETaskType::Testing:
        TestingTaskCount += Value;
        OnTestingTaskCount();
        return;
    case ETaskType::Server:
        ServerFinishTaskCount += Value;
        OnServerFinishTaskCount();
        return;
    case ETaskType::Bug:
        BugCount += Value;
        OnBugCountChange();
        OnBugCountChange_Implementation();
        return;
    }
}

void AFDGameState::OnRep_BugCount()
{
    OnBugCountChange();
    OnBugCountChange_Implementation();
}

void AFDGameState::OnRep_ServerFinishTaskCount()
{
    OnServerFinishTaskCount();
}

void AFDGameState::OnServerFinishTaskCount_Implementation()
{
}

void AFDGameState::OnRep_ArtTaskCount()
{
    OnArtTaskCount();
}

void AFDGameState::OnArtTaskCount_Implementation()
{
}

void AFDGameState::OnRep_DevelopingTaskCount()
{
    OnDevelopingTaskCount();
}

void AFDGameState::OnDevelopingTaskCount_Implementation()
{
}

void AFDGameState::OnRep_TestingTaskCount()
{
    OnTestingTaskCount();
}

void AFDGameState::OnTestingTaskCount_Implementation()
{
}

void AFDGameState::OnBugCountChange_Implementation()
{
    // Оповещаем все компьютеры об изменении
    for (TActorIterator<AFDComputerActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
    {
        AFDComputerActor *ComputerActor = *ActorIt;
        if (ComputerActor->Type == ETaskType::Server)
        {
            ComputerActor->UpdateBugs();
            break;
        }
    }
}

void AFDGameState::OnRep_RemainingTime()
{
    OnRemainingTime();
}

void AFDGameState::OnRemainingTime_Implementation()
{
}

void AFDGameState::UpdateGameTimer()
{
    if (GetLocalRole() == ROLE_Authority)
    {
        RemainingTime--;
        OnRemainingTime();

        if (RemainingTime <= 0)
        {
            GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
            // Оповещаем GameMode о завершении
            if (AFDGameMode *GM = Cast<AFDGameMode>(GetWorld()->GetAuthGameMode()))
            {
                GM->EndGame();
            }
        }
    }
}

void AFDGameState::ShowWidgetToAllPlayers()
{
    if (!EndGameWidgetClass)
    {
        return;
    }

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController *PC = It->Get();
        if (PC && PC->IsLocalController()) // Критически важно!
        {

            UUserWidget *Widget = CreateWidget<UUserWidget>(PC, EndGameWidgetClass);
            if (Widget)
            {
                Widget->AddToViewport();
            }
        }
    }
}

void AFDGameState::MulticastEndGame_Implementation()
{
    ShowWidgetToAllPlayers();

    // 2. Отключаем управление всем игрокам
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (APlayerController *PC = It->Get())
        {
            if (PC->IsLocalController())
            {
                // Отключаем ввод
                PC->SetIgnoreMoveInput(true);
                PC->SetIgnoreLookInput(true);

                // Блокируем управление персонажем
                if (APawn *ControlledPawn = PC->GetPawn())
                {
                    ControlledPawn->DisableInput(PC);
                }

                // Показываем курсор
                PC->bShowMouseCursor = true;
                PC->SetInputMode(FInputModeUIOnly());
            }
        }
    }
}