// Fill out your copyright notice in the Description page of Project Settings.

#include "FDBacklogActor.h"
#include "FDCharacter.h"
#include "FDTaskActor.h"
#include "FDPlayerController.h"
#include "Components/BoxComponent.h"

// Sets default values
AFDBacklogActor::AFDBacklogActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	RootComponent = InteractionVolume;
	InteractionVolume->SetCollisionProfileName(TEXT("Trigger"));
	InteractionVolume->SetBoxExtent(FVector(100.0f));

	bReplicates = true;
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void AFDBacklogActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFDBacklogActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFDBacklogActor::NotifyActorBeginOverlap(AActor *OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (AFDCharacter *Character = Cast<AFDCharacter>(OtherActor))
	{
		AFDPlayerController *PlayerController = Cast<AFDPlayerController>(Character->GetController());

		if (PlayerController && PlayerController->GetIsCarrying() && CanAcceptTask(PlayerController->GetCurrentTask()))
		{
			PlayerController->DropCarriedItem();
		}
	}
}

bool AFDBacklogActor::CanAcceptTask(const AFDTaskActor *Task) const
{
	if (!Task)
		return false;

	// Если AcceptedTypes пустой - принимаем все типы
	if (AcceptedTypes.Num() == 0)
		return true;

	// Проверяем есть ли тип задачи в AcceptedTypes
	return AcceptedTypes.Contains(Task->Type);
}
