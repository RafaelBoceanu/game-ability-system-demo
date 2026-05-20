#include "GASDemoEnemy.h"
#include "GameplayEffectExtension.h"
#include "Components/CapsuleComponent.h"

AGASDemoEnemy::AGASDemoEnemy()
{
    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(
        TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
}

UAbilitySystemComponent* AGASDemoEnemy::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void AGASDemoEnemy::BeginPlay()
{
    Super::BeginPlay();

    if (AbilitySystemComponent)
    {
		UGASDemoAttributeSet* NewAttributeSet = NewObject<UGASDemoAttributeSet>(this);
        AbilitySystemComponent->AddAttributeSetSubobject(NewAttributeSet);
		
        AttributeSet = AbilitySystemComponent->GetSet<UGASDemoAttributeSet>();

        if (DefaultAttributes)
        {
            FGameplayEffectContextHandle Context =
                AbilitySystemComponent->MakeEffectContext();
            FGameplayEffectSpecHandle Spec =
                AbilitySystemComponent->MakeOutgoingSpec(
                    DefaultAttributes, 1, Context);
            AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
                *Spec.Data.Get());
        }

        // Listen for health changes
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            UGASDemoAttributeSet::GetHealthAttribute())
            .AddUObject(this, &AGASDemoEnemy::OnHealthChanged);
    }
}

void AGASDemoEnemy::OnHealthChanged(const FOnAttributeChangeData& Data)
{
    UE_LOG(LogTemp, Warning, TEXT("Enemy %s health changed: %.1f -> %.1f"),
        *GetName(), Data.OldValue, Data.NewValue);

    if (Data.NewValue <= 0.f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemy %s is dead!"), *GetName());

        // Ragdoll
        GetMesh()->SetSimulatePhysics(true);
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        // Destroy after 3 seconds
        SetLifeSpan(3.f);
    }
}