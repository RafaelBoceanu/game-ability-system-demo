#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "GASDemoProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class GASDEMO_API AGASDemoProjectile : public AActor
{
    GENERATED_BODY()

public:
    AGASDemoProjectile();

    UPROPERTY(VisibleAnywhere, Category = "Projectile")
    USphereComponent* CollisionComp;

    UPROPERTY(VisibleAnywhere, Category = "Projectile")
    UProjectileMovementComponent* ProjectileMovement;

    // The damage effect to apply on hit
    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    // Who fired this — used to apply the effect from the right source
    UPROPERTY()
    UAbilitySystemComponent* SourceASC;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);
};