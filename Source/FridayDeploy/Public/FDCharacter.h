// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FDCharacter.generated.h"

UCLASS()
class FRIDAYDEPLOY_API AFDCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFDCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent *SpringArm;

	// Camera that will follow the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent *FollowCamera;

	// Called when an actor is overlapping the interaction area
	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;

	UFUNCTION()
	virtual void NotifyActorEndOverlap(AActor *OtherActor) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
};
