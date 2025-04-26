// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FDBacklogTypes.h"
#include "GameFramework/Character.h"
#include "FDCharacter.generated.h"

UCLASS()
class FRIDAYDEPLOY_API AFDCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFDCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	UAudioComponent *FootstepAudioComp;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlayFootstepAudio();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayFootstepAudio();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopFootstepAudio();

	// Мультикаст функция
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StopFootstepAudio();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	UAudioComponent *PushingAudioComp;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlayPushingAudio();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayPushingAudio();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StopPushingAudio();

	// Мультикаст функция
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StopPushingAudio();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	UAudioComponent *TaskCompleteAudioComp;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlayCompleteAudio();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayCompleteAudio();

	UFUNCTION()
	bool IsCurrentComputerActor(AFDComputerActor *ComputerActor) { return CurrentComputerActor == ComputerActor; };

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetCurrentComputerActor(AFDComputerActor *ComputerActor) { CurrentComputerActor = ComputerActor; }

	UFUNCTION(BlueprintPure, Category = "Carrying")
	bool GetIsCarrying() { return bIsCarrying; }

	UFUNCTION(BlueprintCallable, Category = "Carrying")
	void DropCarriedItem(EBacklogType BacklogType);

	UFUNCTION()
	AFDTaskActor *GetCurrentTask() { return CurrentTaskActor; };

	UFUNCTION(Server, Reliable, Category = "Score")
	void Server_ChangeTaskCountByType(ETaskType TaskType, int32 Value);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

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

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 45.0f; // Degrees per second

	UPROPERTY(VisibleAnywhere, Category = "Task")
	AFDTaskActor *CurrentTaskActor;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Interact();

	// Multicast функция для всех клиентов
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnInteractionComplete();

	// Movement functions
	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void OnMovement(float Value);

	// Interaction
	UFUNCTION()
	void Interact();

	UFUNCTION()
	void FinishInteraction();

	// Set the art actor that is currently in range for the character to interact with

	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool GetCanInteract() { return bCanInteract; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool GetIsInteracting() { return bIsInteracting; }

	// The art actor that is currently in range for the character to interact with
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Interaction")
	AFDComputerActor *CurrentComputerActor;

	// Whether the character can interact with an art actor
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Interaction")
	bool bCanInteract = false;

	// Whether the character is currently interacting with an art actor
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Interaction")
	bool bIsInteracting = false;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Carrying")
	bool bIsCarrying = false;

private:
	// Timer handle for the interaction timer
	FTimerHandle InteractionTimerHandle;

	UFUNCTION()
	void ExecuteInteraction();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
};
