#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Runtime/UMG/Public/UMG.h"
#include "PlayerHUD.generated.h"

UCLASS()
class HUDANDDAMAGE_API UPlayerHUD : public UUserWidget {
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UCanvasPanel* ScreenBorder;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UCanvasPanel* CompassBorder;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UImage* Crosshair;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UImage* CompassMarker;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UImage* CompassLegend;

	UCanvasPanelSlot* CompassLegendSlot;

public:
	virtual void NativeConstruct() override;

	void OrientLegend(float Yaw);

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UProgressBar* ShieldBar;
};
