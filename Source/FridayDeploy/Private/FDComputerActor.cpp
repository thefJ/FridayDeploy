// Fill out your copyright notice in the Description page of Project Settings.

#include "FDComputerActor.h"
#include "Components/BoxComponent.h"
#include "FDGameState.h"
#include "Kismet/KismetMathLibrary.h"

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

void AFDComputerActor::UpdateBugs()
{
	if (AFDGameState *GS = GetWorld()->GetGameState<AFDGameState>())
	{
		int32 TargetCount = FMath::CeilToInt(GS->BugCount * ViewBugCoefficient);

		if (TargetCount > CurrentBugs.Num())
		{
			SpawnBugs(TargetCount - CurrentBugs.Num());
		}
		else if (TargetCount < CurrentBugs.Num())
		{
			RemoveBugs(CurrentBugs.Num() - TargetCount);
		}
	}
}
void AFDComputerActor::SpawnBugs(int32 Count)
{
	if (!Bug)
	{
		return;
	}

	for (int32 i = 0; i < Count; ++i)
	{
		FVector SpawnLocation = GetActorLocation() +
								FVector(
									FMath::FRandRange(-50.f, 50.f),
									FMath::FRandRange(-50.f, 50.f),
									FMath::FRandRange(10.f, 30.f));
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		// GetWorld()->SpawnActor<AFDTaskActor>(CurrentComputerActor->GetTaskActor(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		auto NewBug = GetWorld()->SpawnActor<AFDBugActor>(Bug, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

		if (NewBug)
		{
			CurrentBugs.Add(NewBug);
		}
	}
}
void AFDComputerActor::RemoveBugs(int32 Count)
{
	Count = FMath::Min(Count, CurrentBugs.Num());

	for (int32 i = 0; i < Count; ++i)
	{
		if (CurrentBugs.Num() > 0)
		{
			CurrentBugs.Last()->Destroy();
			CurrentBugs.Pop();
		}
	}
}