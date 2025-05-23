// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/Character.h"
#include "FDGameMode.generated.h"

/**
 *
 */
UCLASS()
class FRIDAYDEPLOY_API AFDGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AFDGameMode();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> WaitingWidgetClass;

	UFUNCTION()
	void EndGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters")
	TArray<TSubclassOf<ACharacter>> AvailableCharacterClasses;

	// Текущий индекс для распределения
	UPROPERTY(Replicated)
	int32 CurrentCharacterIndex = 0;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Setting")
	int32 MinPlayerCount = 2;

	virtual bool ReadyToStartMatch_Implementation() override;

	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;

	virtual void PostLogin(APlayerController *NewPlayer) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
	virtual AActor *ChoosePlayerStart_Implementation(AController *Player) override;

private:
	UPROPERTY()
	UUserWidget *WaitingWidgetInstance;
};
