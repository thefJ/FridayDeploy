// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FDTaskTypes.h"
#include "GameFramework/Actor.h"
#include "FDBacklogActor.generated.h"

UCLASS()
class FRIDAYDEPLOY_API AFDBacklogActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFDBacklogActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Backlog")
	TArray<ETaskType> AcceptedTypes;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent *InteractionVolume;

	UFUNCTION()
	void NotifyActorBeginOverlap(AActor *OtherActor);

	UFUNCTION()
	bool CanAcceptTask(const AFDTaskActor *Task) const;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
