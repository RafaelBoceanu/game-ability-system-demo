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
    UE_LOG(LogTemp, Error, TEXT("GA_DASH ACTIVATE ABILITY ENTERED"));
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!Character)
    {
        UE_LOG(LogTemp, Error, TEXT("Dash: Character is null"));
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Dash: Character found: %s"), *Character->GetName());
    UE_LOG(LogTemp, Warning, TEXT("Dash: Controller: %s"),
        Character->GetController() ? *Character->GetController()->GetName() : TEXT("NULL"));

    FVector DashDirection = Character->GetActorForwardVector();
    UE_LOG(LogTemp, Warning, TEXT("Dash: Direction: %s, Strength: %f"),
        *DashDirection.ToString(), DashStrength);

    Character->LaunchCharacter(DashDirection * DashStrength, true, true);
    UE_LOG(LogTemp, Warning, TEXT("Dash: LaunchCharacter called"));

    FTimerHandle TimerHandle;
    FGameplayAbilitySpecHandle CapturedHandle = Handle;
    FGameplayAbilityActivationInfo CapturedActivation = ActivationInfo;

    Character->GetWorldTimerManager().SetTimer(TimerHandle, [this, ActorInfo,
        CapturedHandle, CapturedActivation]()
        {
            EndAbility(CapturedHandle, ActorInfo, CapturedActivation, true, false);
        }, DashDuration, false);
}