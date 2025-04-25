// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "FDGameMode.generated.h"

/**
 *
 */
UCLASS()
class FRIDAYDEPLOY_API AFDGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> EndGameWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowWidgetToAllPlayers();

	void EndGame();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	int32 MinPlayerCount = 2;

	virtual bool ReadyToStartMatch_Implementation() override;
};
