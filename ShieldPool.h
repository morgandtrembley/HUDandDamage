#pragma once
#include "CoreMinimal.h"
#include "Pool.h"
#include "ShieldPool.generated.h"

UCLASS()
class HUDANDDAMAGE_API AShieldPool : public APool {
	GENERATED_BODY()
	
	AShieldPool();

	virtual void Use(AHUDAndDamageCharacter* Character) override;
};
