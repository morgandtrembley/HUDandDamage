#include "ShieldPool.h"
#include "HUDAndDamageCharacter.h"

AShieldPool::AShieldPool() {

	// Material set up
	static ConstructorHelpers::FObjectFinder<UMaterialInstance>MaterialAsset(TEXT("/Game/1MyContent/Materials/ShieldInst.ShieldInst"));
	Mesh->SetMaterial(0, MaterialAsset.Object);

	Modifier = 0.5f;
}

// handles shield addition and shield overflow if it occurs
void AShieldPool::Use(AHUDAndDamageCharacter* Character) {
	if (Character->Shield < SHIELD_MAX) {
		Character->Shield += Modifier;
	}
	else {
		Character->Shield = SHIELD_MAX;
	}
}