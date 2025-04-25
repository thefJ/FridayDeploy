// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	virtual bool ReadyToStartMatch_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	int32 MinPlayerCount = 2;
};
