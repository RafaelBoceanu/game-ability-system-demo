#pragma once

#include "CoreMinimal.h"
#include "GASDemoGameplayAbility.h"
#include "GameplayEffect.h"
#include "GA_AOESlam.generated.h"

UCLASS()
class GASDEMO_API UGA_AOESlam : public UGASDemoGameplayAbility
{
    GENERATED_BODY()

public:
    UGA_AOESlam();

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    UPROPERTY(EditDefaultsOnly, Category = "AOESlam")
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, Category = "AOESlam")
    float SlamRadius = 500.f;

    UPROPERTY(EditDefaultsOnly, Category = "AOESlam")
    float WindupTime = 0.3f;
};