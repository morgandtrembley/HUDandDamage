#include "DamagePool.h"
#include "HUDAndDamageCharacter.h"

ADamagePool::ADamagePool() {
	
	// material set up
	static ConstructorHelpers::FObjectFinder<UMaterialInstance>MaterialAsset(TEXT("/Game/1MyContent/Materials/DamageInst.DamageInst"));
	Mesh->SetMaterial(0, MaterialAsset.Object);

	Modifier = 1.0f;
}

void ADamagePool::Use(AHUDAndDamageCharacter* Character) {
	Character->TakeSomeDamage(Modifier);
}
