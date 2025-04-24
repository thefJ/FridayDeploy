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

protected:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 45.0f; // Degrees per second

	UPROPERTY(EditAnywhere, Category = "Interact")
	float BaseInteractTime = 3.0f; // Degrees per second

	UPROPERTY(VisibleAnywhere, Category = "Task")
	AFDTaskActor *CurrentTaskActor;

	// Movement functions
	void MoveForward(float Value);
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

private:
	// The art actor that is currently in range for the character to interact with
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	AFDComputerActor *CurrentComputerActor;

	// Whether the character can interact with an art actor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bCanInteract = false;

	// Whether the character is currently interacting with an art actor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bIsInteracting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Carrying", meta = (AllowPrivateAccess = "true"))
	bool bIsCarrying = false;

	// Timer handle for the interaction timer
	FTimerHandle InteractionTimerHandle;
};
