#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamagableObject.generated.h"

class UDamageNumbers;
class ADamageNumbersActor;

UCLASS()
class HUDANDDAMAGE_API ADamagableObject : public AActor {
	GENERATED_BODY()
	
public:	
	ADamagableObject();

	void GetHit(int Damage, FVector Location);

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UClass* DamageNumbersActorClass;

	ADamageNumbersActor* DamageNumbersActor;

	int Health;
};
