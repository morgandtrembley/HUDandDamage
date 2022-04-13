#pragma once
#include "CoreMinimal.h"
#include "Pool.h"
#include "DamagePool.generated.h"

UCLASS()
class HUDANDDAMAGE_API ADamagePool : public APool {
	GENERATED_BODY()

	ADamagePool();

	virtual void Use(AHUDAndDamageCharacter* Character) override;
};