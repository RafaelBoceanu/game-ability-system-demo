#include "GA_Shield.h"
#include "AbilitySystemComponent.h"

UGA_Shield::UGA_Shield()
{
    FGameplayTagContainer Tags;
    Tags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Shield"));
    SetAssetTags(Tags);
}

void UGA_Shield::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!ShieldEffectClass)
    {
        ShieldEffectClass = StaticLoadClass(UGameplayEffect::StaticClass(), nullptr,
            TEXT("/Game/GASDemo/Enemies/GE_Shield.GE_Shield_C"));
    }

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    if (ShieldEffectClass && ActorInfo->AbilitySystemComponent.IsValid())
    {
        FGameplayEffectContextHandle Context =
            ActorInfo->AbilitySystemComponent->MakeEffectContext();
        FGameplayEffectSpecHandle Spec =
            ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(
                ShieldEffectClass, 1, Context);
        ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
            *Spec.Data.Get());
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}