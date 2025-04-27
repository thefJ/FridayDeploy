// Fill out your copyright notice in the Description page of Project Settings.

#include "FDGameMode.h"
#include "FDGameState.h"
#include "Net/UnrealNetwork.h"

AFDGameMode::AFDGameMode()
{
    bReplicates = true;
}

bool AFDGameMode::ReadyToStartMatch_Implementation()
{
    // Стандартная проверка (ожидание старта + есть игроки)
    if (!Super::ReadyToStartMatch_Implementation())
    {
        return false;
    }
    // Дополнительная проверка: минимум MinPlayerCount игрока
    return (GetNumPlayers() >= MinPlayerCount);
}

void AFDGameMode::EndGame()
{
    if (HasAuthority()) // Проверяем, что мы на сервере
    {
        // Логика завершения игры на сервере
        // ...

        EndMatch();
        // Отправляем вызов всем клиентам

        if (AFDGameState *GS = GetGameState<AFDGameState>())
        {
            GS->MulticastEndGame();
        }
    }
}

void AFDGameMode::HandleMatchIsWaitingToStart()
{
    Super::HandleMatchIsWaitingToStart();

    // Показываем виджет всем игрокам
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (APlayerController *PC = It->Get())
        {
            if (PC->IsLocalController())
            {
                WaitingWidgetInstance = CreateWidget<UUserWidget>(PC, WaitingWidgetClass);
                if (WaitingWidgetInstance)
                {
                    WaitingWidgetInstance->AddToViewport();

                    // Блокируем игровой ввод
                    PC->SetInputMode(FInputModeUIOnly());
                    PC->bShowMouseCursor = true;
                }
            }
        }
    }
}

void AFDGameMode::HandleMatchHasStarted()
{
    Super::HandleMatchHasStarted();

    // Удаляем виджет ожидания
    if (WaitingWidgetInstance)
    {
        WaitingWidgetInstance->RemoveFromParent();
        WaitingWidgetInstance = nullptr;
    }

    // Восстанавливаем управление
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (APlayerController *PC = It->Get())
        {
            if (PC->IsLocalController())
            {
                PC->SetInputMode(FInputModeGameOnly());
                PC->bShowMouseCursor = false;
            }
        }
    }
}

void AFDGameMode::PostLogin(APlayerController *NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (!NewPlayer || AvailableCharacterClasses.Num() == 0)
        return;

    // Выбираем класс персонажа
    static int32 CharacterIndex = 0;
    TSubclassOf<ACharacter> SelectedClass = AvailableCharacterClasses[CharacterIndex % AvailableCharacterClasses.Num()];
    CharacterIndex++;

    // Выбираем точку спавна
    if (AActor *PlayerStart = ChoosePlayerStart(NewPlayer))
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = NewPlayer;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        // Спавним персонажа
        if (ACharacter *NewCharacter = GetWorld()->SpawnActor<ACharacter>(
                SelectedClass,
                PlayerStart->GetActorLocation(),
                PlayerStart->GetActorRotation(),
                SpawnParams))
        {
            // Удаляем старый пеон если есть
            if (APawn *OldPawn = NewPlayer->GetPawn())
            {
                OldPawn->Destroy();
            }

            // Завладеваем новым персонажем
            NewPlayer->Possess(NewCharacter);
        }
    }
}

AActor *AFDGameMode::ChoosePlayerStart_Implementation(AController *Player)
{
    // Базовая реализация выбора PlayerStart
    return Super::ChoosePlayerStart_Implementation(Player);
}

void AFDGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AFDGameMode, CurrentCharacterIndex);
}