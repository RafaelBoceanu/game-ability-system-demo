#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASDemoGameplayAbility.generated.h"

UCLASS()
class GASDEMO_API UGASDemoGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGASDemoGameplayAbility();

	// Input tag that activates this ability
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FGameplayTag AbilityInputTag;
};