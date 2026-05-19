#include "GASDemoProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AGASDemoProjectile::AGASDemoProjectile()
{
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(15.f);
    CollisionComp->SetCollisionProfileName("Projectile");
    RootComponent = CollisionComp;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(
        TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;

    InitialLifeSpan = 3.f;
}

void AGASDemoProjectile::BeginPlay()
{
    Super::BeginPlay();
    CollisionComp->OnComponentHit.AddDynamic(this, &AGASDemoProjectile::OnHit);
}

void AGASDemoProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == GetOwner()) return;

    UAbilitySystemComponent* TargetASC =
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

    if (TargetASC && SourceASC && DamageEffectClass)
    {
        FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
        FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(
            DamageEffectClass, 1, Context);
        SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
    }

    Destroy();
}