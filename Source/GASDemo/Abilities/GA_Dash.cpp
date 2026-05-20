#include "GA_Dash.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"

UGA_Dash::UGA_Dash()
{
    FGameplayTagContainer Tags;
    Tags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Dash"));
    SetAssetTags(Tags);
}

void UGA_Dash::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!Character)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    FVector DashDirection = Character->GetActorForwardVector();

    Character->LaunchCharacter(DashDirection * DashStrength, true, true);

    FTimerHandle TimerHandle;
    FGameplayAbilitySpecHandle CapturedHandle = Handle;
    FGameplayAbilityActivationInfo CapturedActivation = ActivationInfo;

    Character->GetWorldTimerManager().SetTimer(TimerHandle, [this, ActorInfo,
        CapturedHandle, CapturedActivation]()
        {
            EndAbility(CapturedHandle, ActorInfo, CapturedActivation, true, false);
        }, DashDuration, false);
}