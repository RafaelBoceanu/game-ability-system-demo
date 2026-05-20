#include "GA_Fireball.h"
#include "Projectiles/GASDemoProjectile.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "UObject/ConstructorHelpers.h"

UGA_Fireball::UGA_Fireball()
{
    FGameplayTagContainer Tags;
    Tags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Fireball"));
    SetAssetTags(Tags);

    static ConstructorHelpers::FClassFinder<AGASDemoProjectile> ProjectileBP(
        TEXT("/Game/GASDemo/Enemies/BP_GASDemoProjectile"));
    if (ProjectileBP.Succeeded())
    {
        ProjectileClass = ProjectileBP.Class;
    }
}

void UGA_Fireball::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!Character || !ProjectileClass)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    // Spawn location: slightly in front and above the character
    FVector SpawnLocation = Character->GetActorLocation()
        + Character->GetActorForwardVector() * 120.f
        + FVector(0.f, 0.f, 50.f);

    FRotator SpawnRotation = Character->GetActorForwardVector().Rotation();

    FActorSpawnParameters Params;
    Params.Owner = Character;
    Params.Instigator = Character;
    Params.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AGASDemoProjectile* Projectile = GetWorld()->SpawnActor<AGASDemoProjectile>(
        ProjectileClass, SpawnLocation, SpawnRotation, Params);

    if (Projectile)
        Projectile->SourceASC = ActorInfo->AbilitySystemComponent.Get();

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}