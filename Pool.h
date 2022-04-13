#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Pool.generated.h"

class AHUDAndDamageCharacter;

UCLASS()
class HUDANDDAMAGE_API APool : public AActor {
	GENERATED_BODY()

public:
	APool();

	UFUNCTION()
	virtual void Use(AHUDAndDamageCharacter* Character);

	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	float Modifier;
};