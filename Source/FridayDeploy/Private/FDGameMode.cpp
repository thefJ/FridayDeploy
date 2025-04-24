// Fill out your copyright notice in the Description page of Project Settings.

#include "FDGameMode.h"

void AFDGameMode::PostLogin(APlayerController *NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (AFDPlayerController *PC = Cast<AFDPlayerController>(NewPlayer))
    {
        // Принудительная инициализация
        // PC->ClientInitialized();
    }
}