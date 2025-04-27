#include "FDSlowZoneActor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

AFDSlowZoneActor::AFDSlowZoneActor()
{
    bReplicates = true;
    bAlwaysRelevant = true;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;
    CollisionBox->SetIsReplicated(true);

    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFDSlowZoneActor::OnBeginOverlap);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AFDSlowZoneActor::OnEndOverlap);
}

void AFDSlowZoneActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AFDSlowZoneActor, CollisionBox);
    DOREPLIFETIME(AFDSlowZoneActor, AffectedPawns);
}

void AFDSlowZoneActor::OnBeginOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor,
                                      UPrimitiveComponent *OtherComp, int32 OtherBodyIndex,
                                      bool bFromSweep, const FHitResult &SweepResult)
{
    if (APawn *Pawn = Cast<APawn>(OtherActor))
    {
        if (HasAuthority())
        {
            Server_ApplySlowEffect(Pawn);
        }
        else
        {
            // Для клиента вызываем multicast напрямую
            Multicast_ApplySlowEffect(Pawn);
        }
    }
}

void AFDSlowZoneActor::OnEndOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor,
                                    UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    if (APawn *Pawn = Cast<APawn>(OtherActor))
    {
        if (HasAuthority())
        {
            Server_RemoveSlowEffect(Pawn);
        }
        else
        {
            // Для клиента вызываем multicast напрямую
            Multicast_RemoveSlowEffect(Pawn);
        }
    }
}

void AFDSlowZoneActor::OnRep_SlowEffects()
{
    for (TWeakObjectPtr<APawn> &PawnPtr : AffectedPawns)
    {
        if (APawn *Pawn = PawnPtr.Get())
        {
            ApplySlowEffect(Pawn, true);
        }
    }
}

void AFDSlowZoneActor::ApplySlowEffect(APawn *Pawn, bool bApply)
{
    if (!Pawn)
        return;

    // Определяем множитель скорости
    float Rate = bApply ? 0.5f : 1.0f;

    // 1. Замедляем движение
    if (UCharacterMovementComponent *Movement = Pawn->FindComponentByClass<UCharacterMovementComponent>())
    {
        if (bApply)
        {
            // Сохраняем оригинальную скорость при первом применении
            if (!OriginalSpeeds.Contains(Pawn))
            {
                OriginalSpeeds.Add(Pawn, Movement->MaxWalkSpeed);
            }
            Movement->MaxWalkSpeed = OriginalSpeeds[Pawn] * 0.5f;
        }
        else if (OriginalSpeeds.Contains(Pawn))
        {
            // Восстанавливаем оригинальную скорость
            Movement->MaxWalkSpeed = OriginalSpeeds[Pawn];
            OriginalSpeeds.Remove(Pawn);
        }
    }

    // 2. Замедляем анимации
    if (USkeletalMeshComponent *Mesh = Pawn->FindComponentByClass<USkeletalMeshComponent>())
    {
        if (bApply)
        {
            if (!OriginalAnimRates.Contains(Pawn))
            {
                OriginalAnimRates.Add(Pawn, Mesh->GlobalAnimRateScale);
            }
            Mesh->GlobalAnimRateScale = OriginalAnimRates[Pawn] * 0.5f;
        }
        else if (OriginalAnimRates.Contains(Pawn))
        {
            Mesh->GlobalAnimRateScale = OriginalAnimRates[Pawn];
            OriginalAnimRates.Remove(Pawn);
        }
    }
}

void AFDSlowZoneActor::Multicast_ApplySlowEffect_Implementation(APawn *Pawn)
{
    ApplySlowEffect(Pawn, true);
}

void AFDSlowZoneActor::Multicast_RemoveSlowEffect_Implementation(APawn *Pawn)
{
    ApplySlowEffect(Pawn, false);
}

void AFDSlowZoneActor::Server_RemoveSlowEffect_Implementation(APawn *Pawn)
{
    if (!Pawn || !HasAuthority())
        return;

    Multicast_RemoveSlowEffect(Pawn);
    AffectedPawns.Remove(Pawn);
}

void AFDSlowZoneActor::Server_ApplySlowEffect_Implementation(APawn *Pawn)
{
    if (!Pawn || !HasAuthority())
        return;

    Multicast_ApplySlowEffect(Pawn);
    AffectedPawns.Add(Pawn);
}