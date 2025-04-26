// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "FDGameState.generated.h"

/**
 *
 */
UCLASS()
class FRIDAYDEPLOY_API AFDGameState : public AGameState
{
	GENERATED_BODY()

public:
	AFDGameState();

	UPROPERTY(ReplicatedUsing = OnRep_ArtTaskCount, EditAnywhere, BlueprintReadWrite, Category = "Task Tracking")
	int32 ArtTaskCount;

	UPROPERTY(ReplicatedUsing = OnRep_DevelopingTaskCount, EditAnywhere, BlueprintReadWrite, Category = "Task Tracking")
	int32 DevelopingTaskCount;

	UPROPERTY(ReplicatedUsing = OnRep_TestingTaskCount, EditAnywhere, BlueprintReadWrite, Category = "Task Tracking")
	int32 TestingTaskCount;

	UPROPERTY(ReplicatedUsing = OnRep_BugCount, EditAnywhere, BlueprintReadWrite, Category = "Task Tracking")
	int32 BugCount;

	UPROPERTY(ReplicatedUsing = OnRep_ServerFinishTaskCount, EditAnywhere, BlueprintReadWrite, Category = "Task Tracking")
	int32 ServerFinishTaskCount;

	UFUNCTION(BlueprintCallable, Category = "Tasks")
	int32 GetTaskCountByType(ETaskType TaskType) const;

	UFUNCTION(BlueprintCallable, Category = "Tasks")
	void ChangeTaskCountByType(ETaskType TaskType, int32 value);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	UFUNCTION(BlueprintNativeEvent)
	void OnBugCountChange();
	virtual void OnBugCountChange_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnServerFinishTaskCount();

	UFUNCTION(BlueprintNativeEvent)
	void OnArtTaskCount();

	UFUNCTION(BlueprintNativeEvent)
	void OnDevelopingTaskCount();

	UFUNCTION(BlueprintNativeEvent)
	void OnTestingTaskCount();

	UFUNCTION(BlueprintNativeEvent)
	void OnRemainingTime();

	UPROPERTY(ReplicatedUsing = OnRep_RemainingTime, EditAnywhere, BlueprintReadWrite, Category = "Timer")
	int32 RemainingTime = 300; // 5 минут

	// Обновление таймера
	void UpdateGameTimer();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_BugCount();

	UFUNCTION()
	void OnRep_ServerFinishTaskCount();

	UFUNCTION()
	void OnRep_ArtTaskCount();

	UFUNCTION()
	void OnRep_DevelopingTaskCount();

	UFUNCTION()
	void OnRep_TestingTaskCount();

	UFUNCTION()
	void OnRep_RemainingTime();

private:
	FTimerHandle TimerHandle;
};
