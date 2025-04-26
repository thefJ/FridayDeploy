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

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> WaitingWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowWidgetToAllPlayers();

	void EndGame();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	int32 MinPlayerCount = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Game Time")
	float WarmupTime = 10.f; // 10 секунд на подготовку

	virtual bool ReadyToStartMatch_Implementation() override;

	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;

private:
	UPROPERTY()
	UUserWidget *WaitingWidgetInstance;
};
