#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FDSlowZoneActor.generated.h"

UCLASS()
class FRIDAYDEPLOY_API AFDSlowZoneActor : public AActor
{
    GENERATED_BODY()

public:
    AFDSlowZoneActor();

    UPROPERTY(ReplicatedUsing = OnRep_SlowEffects)
    TArray<TWeakObjectPtr<APawn>> AffectedPawns;

    UFUNCTION(Server, Reliable)
    void Server_RemoveSlowEffect(APawn *Pawn);

    // Сетевые функции
    UFUNCTION(Server, Reliable)
    void Server_ApplySlowEffect(APawn *Pawn);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
    class UBoxComponent *CollisionBox;

    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor,
                        UPrimitiveComponent *OtherComp, int32 OtherBodyIndex,
                        bool bFromSweep, const FHitResult &SweepResult);

    UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor,
                      UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_ApplySlowEffect(APawn *Pawn);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_RemoveSlowEffect(APawn *Pawn);

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

    UFUNCTION()
    void OnRep_SlowEffects();

private:
    TMap<TWeakObjectPtr<APawn>, float> OriginalSpeeds;
    TMap<TWeakObjectPtr<APawn>, float> OriginalAnimRates;

    void ApplySlowEffect(APawn *Pawn, bool bApply);
};
