#include "PlayerHUD.h"

void UPlayerHUD::NativeConstruct() {
	Super::NativeConstruct();

	// slot compass legend 
	CompassLegendSlot = Cast<UCanvasPanelSlot>(CompassLegend->Slot);
}

void UPlayerHUD::OrientLegend(float Yaw) {
	float Position;

	// Input is 0 -> 360 but legend is set to rotate -180 -> 180
	if (Yaw > 180) {
		Yaw -= 360;
	}

	// scale to move legend appropriate amount
	Position = -Yaw * 10;
	CompassLegendSlot->SetPosition(FVector2D(Position, 0));
}