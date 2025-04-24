// Fill out your copyright notice in the Description page of Project Settings.

#include "FDGameMode.h"

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