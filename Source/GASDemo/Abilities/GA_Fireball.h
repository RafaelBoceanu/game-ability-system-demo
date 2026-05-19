#pragma once

#include "CoreMinimal.h"
#include "GASDemoGameplayAbility.h"
#include "Projectiles/GASDemoProjectile.h"
#include "GA_Fireball.generated.h"

UCLASS()
class GASDEMO_API UGA_Fireball : public UGASDemoGameplayAbility
{
    GENERATED_BODY()

public:
    UGA_Fireball();

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    UPROPERTY(EditDefaultsOnly, Category = "Fireball")
    TSubclassOf<AGASDemoProjectile> ProjectileClass;
};