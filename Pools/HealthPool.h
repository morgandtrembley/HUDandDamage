#pragma once
#include "CoreMinimal.h"
#include "Pool.h"
#include "HealthPool.generated.h"

UCLASS()
class HUDANDDAMAGE_API AHealthPool : public APool {
	GENERATED_BODY()
	
	AHealthPool();

	virtual void Use(AHUDAndDamageCharacter* Character) override;
};
