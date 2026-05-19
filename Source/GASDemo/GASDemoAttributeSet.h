#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GASDemoAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class GASDEMO_API UGASDemoAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UGASDemoAttributeSet();

    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UGASDemoAttributeSet, Health)

        UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UGASDemoAttributeSet, MaxHealth)

        UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData Mana;
    ATTRIBUTE_ACCESSORS(UGASDemoAttributeSet, Mana)

        UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData MaxMana;
    ATTRIBUTE_ACCESSORS(UGASDemoAttributeSet, MaxMana)

        virtual void PostGameplayEffectExecute(
            const FGameplayEffectModCallbackData& Data) override;
};