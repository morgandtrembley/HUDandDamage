#include "HealthPool.h"
#include "HUDAndDamageCharacter.h"

AHealthPool::AHealthPool() {

	// material set up
	static ConstructorHelpers::FObjectFinder<UMaterialInstance>MaterialAsset(TEXT("/Game/1MyContent/Materials/HealthInst.HealthInst"));
	Mesh->SetMaterial(0, MaterialAsset.Object);

	Modifier = 0.5f;
}

// handles health addition and health overflow if it occurs
void AHealthPool::Use(AHUDAndDamageCharacter* Character) {
	if (Character->Health < HEALTH_MAX) {
		Character->Health += Modifier;
	}
	else {
		Character->Health = HEALTH_MAX;
	}
}