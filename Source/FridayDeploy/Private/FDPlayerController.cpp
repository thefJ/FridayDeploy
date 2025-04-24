// Fill out your copyright notice in the Description page of Project Settings.

#include "FDTaskActor.h"
#include "FDComputerActor.h"
#include "FDPlayerController.h"
#include "FDCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

AFDPlayerController::AFDPlayerController()
{
    bCanInteract = false;
    bIsInteracting = false;
}

void AFDPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    // Set up gameplay key bindings
    if (InputComponent)
    {
        InputComponent->BindAxis("MoveForward", this, &AFDPlayerController::MoveForward);
        InputComponent->BindAxis("MoveRight", this, &AFDPlayerController::MoveRight);
        InputComponent->BindAction("Interact", IE_Pressed, this, &AFDPlayerController::Interact);
    }
}
void AFDPlayerController::MoveForward(float Value)
{
    if (Value != 0.0f && GetPawn() && !bIsInteracting)
    {
        // Get forward vector from controller's rotation (ignoring pitch/roll)
        const FRotator Rotation = GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Get forward vector and move the character
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        GetPawn()->AddMovementInput(Direction, Value);
    }
}
void AFDPlayerController::MoveRight(float Value)
{
    if (Value != 0.0f && GetPawn() && !bIsInteracting)
    {
        // Get right vector from controller's rotation (ignoring pitch/roll)
        const FRotator Rotation = GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Get right vector and move the character
        GetPawn()->AddControllerYawInput(Value * TurnRate * GetWorld()->GetDeltaSeconds());
    }
}

void AFDPlayerController::Interact()
{
    if (bCanInteract && !bIsInteracting && CurrentComputerActor && !bIsCarrying)
    {
        bIsInteracting = true;

        AFDCharacter *FDCharacter = Cast<AFDCharacter>(GetCharacter());

        if (FDCharacter)
        {
            // Disable movement
            if (FDCharacter->GetCharacterMovement())
            {
                FDCharacter->GetCharacterMovement()->DisableMovement();
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Interact: CharacterMovementComponent is null!"));
                return;
            }

            // Start timer for 3 seconds
            GetWorld()->GetTimerManager().SetTimer(
                InteractionTimerHandle,
                this,
                &AFDPlayerController::FinishInteraction,
                BaseInteractTime,
                false);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Interact: Character is null!"));
            return;
        }
    }
}

void AFDPlayerController::FinishInteraction()
{
    AFDCharacter *FDCharacter = Cast<AFDCharacter>(GetCharacter());

    if (FDCharacter)
    {
        // Enable movement
        if (FDCharacter->GetCharacterMovement())
        {
            FDCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("FinishInteraction: CharacterMovementComponent is null!"));
            return;
        }

        if (!CurrentComputerActor || !CurrentComputerActor->GetTaskActor())
            return;
        // Уничтожаем предыдущий актор, если есть
        if (CurrentTaskActor)
        {
            CurrentTaskActor->Destroy();
        }
        // Создаем новый актор
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = FDCharacter;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        CurrentTaskActor = GetWorld()->SpawnActor<AFDTaskActor>(CurrentComputerActor->GetTaskActor(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        // Прикрепляем к руке
        if (CurrentTaskActor && FDCharacter->GetMesh())
        {
            CurrentTaskActor->AttachToComponent(FDCharacter->GetMesh(),
                                                FAttachmentTransformRules::KeepRelativeTransform,
                                                TEXT("CarrySocket")); // Имя сокета для правой руки

            bIsCarrying = true;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("FinishInteraction: AFDCharacter is null!"));
        return;
    }

    bIsInteracting = false;
}