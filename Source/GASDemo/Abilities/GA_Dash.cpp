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

    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // Add invulnerability tag during dash
    FGameplayTagContainer InvulnTag;
    InvulnTag.AddTag(FGameplayTag::RequestGameplayTag("State.Invulnerable"));
    ActorInfo->AbilitySystemComponent->AddLooseGameplayTags(InvulnTag);

    // Launch in the direction the character is facing
    FVector DashDirection = Character->GetActorForwardVector();
    Character->LaunchCharacter(DashDirection * DashStrength, true, true);

    // Remove invulnerability and end ability after dash duration
    FTimerHandle TimerHandle;
    FGameplayAbilitySpecHandle CapturedHandle = Handle;
    FGameplayAbilityActivationInfo CapturedActivation = ActivationInfo;

    Character->GetWorldTimerManager().SetTimer(TimerHandle, [this, ActorInfo,
        CapturedHandle, CapturedActivation, InvulnTag]()
    {
        if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
            ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTags(InvulnTag);

        EndAbility(CapturedHandle, ActorInfo, CapturedActivation, true, false);
    }, DashDuration, false);
}