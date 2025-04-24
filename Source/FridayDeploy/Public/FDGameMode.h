// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FDPlayerController.h"
#include "GameFramework/GameMode.h"
#include "FDGameMode.generated.h"

/**
 *
 */
UCLASS()
class FRIDAYDEPLOY_API AFDGameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void PostLogin(APlayerController *NewPlayer) override;
};
