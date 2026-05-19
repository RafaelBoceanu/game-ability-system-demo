#pragma once

#include "CoreMinimal.h"
#include "GASDemoGameplayAbility.h"
#include "GA_Dash.generated.h"

UCLASS()
class GASDEMO_API UGA_Dash : public UGASDemoGameplayAbility
{
    GENERATED_BODY()

public:
    UGA_Dash();

    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

    UPROPERTY(EditDefaultsOnly, Category = "Dash")
    float DashStrength = 6000.f;

    UPROPERTY(EditDefaultsOnly, Category = "Dash")
    float DashDuration = 0.2f;
};