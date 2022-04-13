#include "DamagableObject.h"
#include "HUDAndDamageCharacter.h"
#include "DamageNumbersActor.h"

ADamagableObject::ADamagableObject() {

	// visual mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/1MyContent/Meshes/CubeMesh.CubeMesh"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetStaticMesh(MeshAsset.Object);

	Tags.Add(FName("Damagable"));

	Health = 100;

	// damage display when hit by player character
	static ConstructorHelpers::FObjectFinder<UClass> DamageNumbersActorBPClass(TEXT("/Game/1MyContent/Blueprints/DamageNumbersActorBP.DamageNumbersActorBP_C"));
	DamageNumbersActorClass = DamageNumbersActorBPClass.Object;

}

void ADamagableObject::GetHit(int Damage, FVector Location) {
	
	// spawn damage numbers
	DamageNumbersActor = Cast<ADamageNumbersActor>(GetWorld()->SpawnActor<AActor>(DamageNumbersActorClass, Location, FRotator(0)));

	// call blueprint function to edit display text and play animation then destroy
	DamageNumbersActor->InitDamage(Damage);
	
	Health -= Damage;
	if (Health <= 0) {
		Destroy();
	}
}