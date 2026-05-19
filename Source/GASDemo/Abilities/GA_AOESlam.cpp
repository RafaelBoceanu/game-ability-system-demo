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

    // Short windup before the slam hits
    FTimerHandle TimerHandle;
    Avatar->GetWorldTimerManager().SetTimer(TimerHandle, [this, Avatar,
        ActorInfo, CapturedHandle, CapturedActivation]()
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

            if (TargetASC && DamageEffectClass)
            {
                FGameplayEffectContextHandle Context =
                    ActorInfo->AbilitySystemComponent->MakeEffectContext();
                FGameplayEffectSpecHandle Spec =
                    ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(
                        DamageEffectClass, 1, Context);
                ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
                    *Spec.Data.Get(), TargetASC);
            }
        }

        EndAbility(CapturedHandle, ActorInfo, CapturedActivation, true, false);
    }, WindupTime, false);
}