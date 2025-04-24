// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FDPlayerController.generated.h"

class AFDTaskActor;

/**
 *
 */
UCLASS()
class FRIDAYDEPLOY_API AFDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFDPlayerController();

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	UFUNCTION()
	bool IsCurrentComputerActor(AFDComputerActor *ComputerActor) { return CurrentComputerActor == ComputerActor; };

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetCurrentComputerActor(AFDComputerActor *ComputerActor) { CurrentComputerActor = ComputerActor; }

	UFUNCTION()
	void SetBCanInteract(bool isCanInteract) { bCanInteract = isCanInteract; };

	UFUNCTION(BlueprintPure, Category = "Carrying")
	bool GetIsCarrying() { return bIsCarrying; }

	UFUNCTION(BlueprintCallable, Category = "Carrying")
	void DropCarriedItem();

	UFUNCTION()
	AFDTaskActor *GetCurrentTask() { return CurrentTaskActor; };

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 45.0f; // Degrees per second

	UPROPERTY(EditAnywhere, Category = "Interact")
	float BaseInteractTime = 3.0f; // Degrees per second

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

	virtual void BeginPlay() override;

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
};
