// Fill out your copyright notice in the Description page of Project Settings.

#include "FDComputerActor.h"
#include "Components/BoxComponent.h"

// Sets default values
AFDComputerActor::AFDComputerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create and setup interaction volume
	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	RootComponent = InteractionVolume;
	InteractionVolume->SetCollisionProfileName(TEXT("Trigger"));
	InteractionVolume->SetBoxExtent(FVector(100.0f));

	bReplicates = true;
    bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void AFDComputerActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFDComputerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
