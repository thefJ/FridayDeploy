// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FDTaskTypes.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent *InteractionVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFDTaskActor> TaskActor;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
