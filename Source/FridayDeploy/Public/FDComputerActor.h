// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FDTaskTypes.h"
#include "FDBugActor.h"
#include "GameFramework/Actor.h"
#include "FDComputerActor.generated.h"

UCLASS()
class FRIDAYDEPLOY_API AFDComputerActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFDComputerActor();

	UFUNCTION()
	TSubclassOf<AFDTaskActor> GetTaskActor() { return TaskActor; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
	ETaskType Type;

	UFUNCTION(BlueprintCallable)
	void UpdateBugs();

protected:
	UPROPERTY(EditAnywhere, Category = "Bug")
	float ViewBugCoefficient = 1.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent *InteractionVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFDTaskActor> TaskActor;

	UPROPERTY()
	TArray<class AFDBugActor *> CurrentBugs;

	// Спавн багов
	void SpawnBugs(int32 Count);

	// Удаление багов
	void RemoveBugs(int32 Count);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFDBugActor> Bug;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
