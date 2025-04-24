// Fill out your copyright notice in the Description page of Project Settings.

#include "FDTaskActor.h"

// Sets default values
AFDTaskActor::AFDTaskActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetActorEnableCollision(false);
}

// Called when the game starts or when spawned
void AFDTaskActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFDTaskActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
