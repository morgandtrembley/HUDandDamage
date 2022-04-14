#include "Pool.h"
#include "HUDAndDamageCharacter.h"

APool::APool() {

	// visual  mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/1MyContent/Meshes/PoolMesh.PoolMesh"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetStaticMesh(MeshAsset.Object);
	Mesh->SetWorldScale3D(FVector(2, 2, 0.1));

	// collision mesh
	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	CollisionSphere->SetupAttachment(Mesh);
	CollisionSphere->SetWorldScale3D(FVector(32));
	CollisionSphere->AddLocalOffset(FVector(0, 0, 5));

	Tags.Add(FName("Pool"));
}

void APool::Use(AHUDAndDamageCharacter* Character) {

}
