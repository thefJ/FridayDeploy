// Fill out your copyright notice in the Description page of Project Settings.

#include "FDCharacter.h"
#include "FDComputerActor.h"
#include "FDTaskActor.h"
#include "FDGameState.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"

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

	if (PlayerInputComponent)
	{
		InputComponent->BindAxis("MoveForward", this, &AFDCharacter::MoveForward);
		InputComponent->BindAxis("MoveRight", this, &AFDCharacter::MoveRight);
		InputComponent->BindAction("Interact", IE_Pressed, this, &AFDCharacter::Interact);
	}
}

void AFDCharacter::NotifyActorBeginOverlap(AActor *OtherActor)
{
	if (AFDComputerActor *ComputerActor = Cast<AFDComputerActor>(OtherActor))
	{
		CurrentComputerActor = ComputerActor;
		bCanInteract = true;
	}
}

void AFDCharacter::NotifyActorEndOverlap(AActor *OtherActor)
{
	if (AFDComputerActor *ComputerActor = Cast<AFDComputerActor>(OtherActor))
	{
		if (IsCurrentComputerActor(ComputerActor))
		{
			CurrentComputerActor = nullptr;
			bCanInteract = false;
		}
	}
}

void AFDCharacter::MoveForward(float Value)
{
	if (Value != 0.0f && !bIsInteracting)
	{
		// Get forward vector from controller's rotation (ignoring pitch/roll)
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector and move the character
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFDCharacter::MoveRight(float Value)
{
	if (Value != 0.0f && !bIsInteracting)
	{
		// Get right vector from controller's rotation (ignoring pitch/roll)
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get right vector and move the character
		AddControllerYawInput(Value * TurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AFDCharacter::ExecuteInteraction()
{
	if (bCanInteract && !bIsInteracting && CurrentComputerActor && !bIsCarrying)
	{
		int32 bugCount = GetWorld()->GetGameState<AFDGameState>()->GetTaskCountByType(ETaskType::Bug);
		if (CurrentComputerActor->Type == ETaskType::Server && bugCount <= 0)
		{
			return;
		}
		else if (CurrentComputerActor->Type != ETaskType::PO && CurrentComputerActor->Type != ETaskType::Server && GetWorld()->GetGameState<AFDGameState>()->GetTaskCountByType(CurrentComputerActor->Type) <= 0)
		{
			return;
		}

		bIsInteracting = true;

		// Disable movement
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->DisableMovement();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Interact: CharacterMovementComponent is null!"));
			return;
		}

		float interactTime = BaseInteractTime + (BaseInteractTime * BugCoefficient * bugCount);
		UE_LOG(LogTemp, Display, TEXT("interactTime %f"), interactTime);

		// Start timer for 3 seconds
		GetWorld()->GetTimerManager().SetTimer(
			InteractionTimerHandle,
			this,
			&AFDCharacter::FinishInteraction,
			interactTime,
			false);

		if (HasAuthority() && CurrentComputerActor->Type != ETaskType::PO)
		{
			if (CurrentComputerActor->Type == ETaskType::Server)
			{
				Server_ChangeTaskCountByType(ETaskType::Bug, -1);
				Server_ChangeTaskCountByType(ETaskType::Server, -1);
			}
			else
			{
				Server_ChangeTaskCountByType(CurrentComputerActor->Type, -1);
			}
		}
	}
}

void AFDCharacter::Interact()
{
	Server_Interact();
}

void AFDCharacter::FinishInteraction()
{
	// Enable movement
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
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
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentTaskActor = GetWorld()->SpawnActor<AFDTaskActor>(CurrentComputerActor->GetTaskActor(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	// Прикрепляем к руке
	if (CurrentTaskActor && GetMesh())
	{
		CurrentTaskActor->AttachToComponent(GetMesh(),
											FAttachmentTransformRules::KeepRelativeTransform,
											TEXT("CarrySocket")); // Имя сокета для правой руки

		bIsCarrying = true;
	}

	bIsInteracting = false;
}

void AFDCharacter::DropCarriedItem(EBacklogType BacklogType)
{
	if (bIsCarrying)
	{
		// Уничтожаем переносимый предмет
		if (CurrentTaskActor)
		{
			if (HasAuthority())
			{
				if (BacklogType == EBacklogType::Server)
				{
					Server_ChangeTaskCountByType(ETaskType::Server, 1);
					int32 chance = CurrentTaskActor->Type == ETaskType::Testing ? BugChanceWitoutTest : BugChanceWithTest;
					if (FMath::RandRange(0, 100) <= chance)
					{
						Server_ChangeTaskCountByType(ETaskType::Bug, 1);
					}
				}
				else
				{
					Server_ChangeTaskCountByType(CurrentTaskActor->Type, 1);
				}
			}

			CurrentTaskActor->Destroy();
			CurrentTaskActor = nullptr;
		}
		// Обновляем состояние
		bIsCarrying = false;

		// Дополнительные действия при сбросе
		// OnItemDropped.Broadcast();
	}
}

bool AFDCharacter::Server_Interact_Validate()
{
	return true;
}

void AFDCharacter::Server_Interact_Implementation()
{
	// Выполняем взаимодействие на сервере
	if (!IsValid(this))
		return;

	ExecuteInteraction();

	// Оповещаем всех клиентов
	Multicast_OnInteractionComplete();
}

void AFDCharacter::Multicast_OnInteractionComplete_Implementation()
{
	// Код, выполняемый на всех клиентах после успешного взаимодействия
	// Например, воспроизведение эффектов, анимаций и т.д.

	// Не выполняем основную логику повторно на сервере
	if (HasAuthority())
		return;

	// Обновляем состояние на клиентах
	ExecuteInteraction();
}

void AFDCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	// Call the Super
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Add properties to replicated for the derived class
	DOREPLIFETIME(AFDCharacter, CurrentComputerActor);
	DOREPLIFETIME(AFDCharacter, bCanInteract);
	DOREPLIFETIME(AFDCharacter, bIsInteracting);
	DOREPLIFETIME(AFDCharacter, bIsCarrying);
}

void AFDCharacter::Server_ChangeTaskCountByType_Implementation(ETaskType TaskType, int32 Value)
{
	if (!IsValid(this))
		return;

	UE_LOG(LogTemp, Warning, TEXT("Server_ChangeTaskCountByType_Implementation"));

	if (AFDGameState *GS = GetWorld()->GetGameState<AFDGameState>())
	{
		GS->ChangeTaskCountByType(TaskType, Value); // GameState сам обрабатывает репликацию
	}
}
