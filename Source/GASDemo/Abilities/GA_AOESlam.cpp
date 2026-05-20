#include "GA_AOESlam.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"

UGA_AOESlam::UGA_AOESlam()
{
    FGameplayTagContainer Tags;
    Tags.AddTag(FGameplayTag::RequestGameplayTag("Ability.AOESlam"));
    SetAssetTags(Tags);
}

void UGA_AOESlam::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!DamageEffectClass)
    {
        DamageEffectClass = StaticLoadClass(UGameplayEffect::StaticClass(), nullptr,
            TEXT("/Game/GASDemo/Enemies/GE_AOEDamage.GE_AOEDamage_C"));
    }

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    AActor* Avatar = ActorInfo->AvatarActor.Get();
    if (!Avatar)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    FGameplayAbilitySpecHandle CapturedHandle = Handle;
    FGameplayAbilityActivationInfo CapturedActivation = ActivationInfo;
    TSubclassOf<UGameplayEffect> CapturedDamageEffect = DamageEffectClass;

    FTimerHandle TimerHandle;
    Avatar->GetWorldTimerManager().SetTimer(TimerHandle, [this, Avatar,
        ActorInfo, CapturedHandle, CapturedActivation, CapturedDamageEffect]()
        {
            if (!Avatar || !ActorInfo) return;

            TArray<FOverlapResult> Overlaps;
            FCollisionShape Sphere = FCollisionShape::MakeSphere(SlamRadius);
            FCollisionQueryParams Params;
            Params.AddIgnoredActor(Avatar);

            GetWorld()->OverlapMultiByChannel(Overlaps, Avatar->GetActorLocation(),
                FQuat::Identity, ECC_Pawn, Sphere, Params);

            for (FOverlapResult& Overlap : Overlaps)
            {
                UAbilitySystemComponent* TargetASC =
                    UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
                        Overlap.GetActor());

                if (TargetASC && CapturedDamageEffect)
                {
                    FGameplayEffectContextHandle Context =
                        ActorInfo->AbilitySystemComponent->MakeEffectContext();
                    FGameplayEffectSpecHandle Spec =
                        ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(
                            CapturedDamageEffect, 1, Context);
                    FActiveGameplayEffectHandle Result =
                        ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
                            *Spec.Data.Get(), TargetASC);
                }
            }

            EndAbility(CapturedHandle, ActorInfo, CapturedActivation, true, false);
        }, WindupTime, false);
}