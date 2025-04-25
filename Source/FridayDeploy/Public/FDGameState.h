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

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Task Tracking")
	int32 ArtTaskCount;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Task Tracking")
	int32 DevelopingTaskCount;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Task Tracking")
	int32 TestingTaskCount;

	UPROPERTY(ReplicatedUsing = OnRep_BugCount, EditAnywhere, BlueprintReadWrite, Category = "Task Tracking")
	int32 BugCount;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Task Tracking")
	int32 ServerFinishTaskCount;

	UFUNCTION(BlueprintCallable, Category = "Tasks")
	int32 GetTaskCountByType(ETaskType TaskType) const;

	UFUNCTION(BlueprintCallable, Category = "Tasks")
	void ChangeTaskCountByType(ETaskType TaskType, int32 value);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_BugCount();

	UFUNCTION()
	void OnBugCountChange();
};
