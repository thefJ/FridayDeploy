// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FDTaskTypes.h"
#include "GameFramework/Actor.h"
#include "FDTaskActor.generated.h"

UCLASS()
class FRIDAYDEPLOY_API AFDTaskActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFDTaskActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
	ETaskType Type;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
