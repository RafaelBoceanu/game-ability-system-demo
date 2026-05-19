#pragma once

#include "CoreMinimal.h"
#include "GASDemoGameplayAbility.h"
#include "GameplayEffect.h"
#include "GA_Shield.generated.h"

UCLASS()
class GASDEMO_API UGA_Shield : public UGASDemoGameplayAbility
{
    GENERATED_BODY()

public:
    UGA_Shield();

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    // Duration GE that grants State.Invulnerable tag
    UPROPERTY(EditDefaultsOnly, Category = "Shield")
    TSubclassOf<UGameplayEffect> ShieldEffectClass;
};