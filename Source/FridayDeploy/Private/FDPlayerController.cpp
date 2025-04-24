// Fill out your copyright notice in the Description page of Project Settings.

#include "FDPlayerController.h"
#include "FDTaskActor.h"
#include "FDComputerActor.h"
#include "FDCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

AFDPlayerController::AFDPlayerController()
{
    bCanInteract = false;
    bIsInteracting = false;
    bReplicates = true;
    bAlwaysRelevant = true;
    SetNetUpdateFrequency(100.0f);
}

void AFDPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (GetLocalRole() == ROLE_SimulatedProxy)
    {
        // Особые действия для клиентских копий
        return;
    }
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

void AFDPlayerController::ExecuteInteraction()
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

void AFDPlayerController::Interact()
{
    if (!IsValid(this))
        return; // Критическая проверка

    if (GetLocalRole() < ROLE_Authority)
    {
        Server_Interact();
        return;
    }
    ExecuteInteraction();
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

void AFDPlayerController::DropCarriedItem()
{
    if (bIsCarrying)
    {
        // Уничтожаем переносимый предмет
        if (CurrentTaskActor)
        {
            CurrentTaskActor->Destroy();
            CurrentTaskActor = nullptr;
        }
        // Обновляем состояние
        bIsCarrying = false;

        // Дополнительные действия при сбросе
        // OnItemDropped.Broadcast();
    }
}

bool AFDPlayerController::Server_Interact_Validate()
{
    return IsValid(this) && GetPawn() != nullptr;
}

void AFDPlayerController::Server_Interact_Implementation()
{
    // Выполняем взаимодействие на сервере
    if (!IsValid(this))
        return;

    ExecuteInteraction();

    // Оповещаем всех клиентов
    Multicast_OnInteractionComplete();
}

void AFDPlayerController::Multicast_OnInteractionComplete_Implementation()
{
    // Код, выполняемый на всех клиентах после успешного взаимодействия
    // Например, воспроизведение эффектов, анимаций и т.д.

    // Не выполняем основную логику повторно на сервере
    if (GetLocalRole() == ROLE_Authority)
        return;

    // Обновляем состояние на клиентах
    ExecuteInteraction();
}

void AFDPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    // Call the Super
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Add properties to replicated for the derived class
    DOREPLIFETIME(AFDPlayerController, CurrentComputerActor);
    DOREPLIFETIME(AFDPlayerController, bCanInteract);
    DOREPLIFETIME(AFDPlayerController, bIsInteracting);
    DOREPLIFETIME(AFDPlayerController, bIsCarrying);
}