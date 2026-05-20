#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "GASDemoAttributeSet.h"
#include "GASDemoEnemy.generated.h"

UCLASS()
class GASDEMO_API AGASDemoEnemy : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AGASDemoEnemy();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
    UAbilitySystemComponent* AbilitySystemComponent;

    UPROPERTY()
    const UGASDemoAttributeSet* AttributeSet;

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
    TSubclassOf<UGameplayEffect> DefaultAttributes;

    // Called when health changes
    void OnHealthChanged(const FOnAttributeChangeData& Data);

protected:
    virtual void BeginPlay() override;
};