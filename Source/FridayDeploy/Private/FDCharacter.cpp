// Fill out your copyright notice in the Description page of Project Settings.

#include "FDCharacter.h"
#include "FDPlayerController.h"
#include "FDComputerActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AFDCharacter::AFDCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 200.0f;	   // Distance from character
	SpringArm->bUsePawnControlRotation = true; // Rotate arm based on controller
	SpringArm->bEnableCameraLag = true;		   // Smooth camera movement
	SpringArm->CameraLagSpeed = 15.0f;		   // Lag speed
	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative
	FollowCamera->SetRelativeRotation(FRotator(0, -5, 0));
	FollowCamera->SetRelativeLocation(FVector(0, 0, 100));

	bReplicates = true;
	bAlwaysRelevant = true;
	SetNetUpdateFrequency(100.0f);
}

// Called when the game starts or when spawned
void AFDCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFDCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFDCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFDCharacter::NotifyActorBeginOverlap(AActor *OtherActor)
{
	AFDPlayerController *FDPlayerController = Cast<AFDPlayerController>(GetController());

	if (!FDPlayerController)
	{
		return;
	}

	if (AFDComputerActor *ComputerActor = Cast<AFDComputerActor>(OtherActor))
	{
		FDPlayerController->SetCurrentComputerActor(ComputerActor);
		FDPlayerController->SetBCanInteract(true);
	}
}

void AFDCharacter::NotifyActorEndOverlap(AActor *OtherActor)
{
	AFDPlayerController *FDPlayerController = Cast<AFDPlayerController>(GetController());

	if (!FDPlayerController)
	{
		return;
	}

	if (AFDComputerActor *ComputerActor = Cast<AFDComputerActor>(OtherActor))
	{
		if (FDPlayerController->IsCurrentComputerActor(ComputerActor))
		{
			FDPlayerController->SetCurrentComputerActor(nullptr);
			FDPlayerController->SetBCanInteract(false);
		}
	}
}
