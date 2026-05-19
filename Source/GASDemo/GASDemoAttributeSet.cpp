#include "GASDemoAttributeSet.h"
#include "GameplayEffectExtension.h"

UGASDemoAttributeSet::UGASDemoAttributeSet()
{
    InitHealth(100.f);
    InitMaxHealth(100.f);
    InitMana(100.f);
    InitMaxMana(100.f);
}

void UGASDemoAttributeSet::PostGameplayEffectExecute(
    const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));

    if (Data.EvaluatedData.Attribute == GetManaAttribute())
        SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
}