// Fill out your copyright notice in the Description page of Project Settings.

#include "FDGameMode.h"
#include "FDGameState.h"

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

void AFDGameMode::ShowWidgetToAllPlayers()
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

void AFDGameMode::EndGame()
{
    // 1. Устанавливаем состояние матча
    EndMatch();

    // 2. Отключаем управление всем игрокам
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (APlayerController *PC = It->Get())
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