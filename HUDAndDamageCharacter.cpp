// Copyright Epic Games, Inc. All Rights Reserved.
#include "HUDAndDamageCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerHUD.h"
#include "DamagableObject.h"
#include "Pool.h"

//////////////////////////////////////////////////////////////////////////
// AHUDAndDamageCharacter

AHUDAndDamageCharacter::AHUDAndDamageCharacter() {
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh(), TEXT("Head"));
	CameraBoom->SetRelativeLocation(FVector(5.0f, 10.0f, 0.0f));
	CameraBoom->TargetArmLength = -5.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Overlap functionality
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AHUDAndDamageCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AHUDAndDamageCharacter::OnOverlapEnd);

	// HUD BP set up
	static ConstructorHelpers::FClassFinder<UUserWidget> HUDBPClass(TEXT("/Game/1MyContent/Blueprints/PlayerHUDBP"));
	HUDClass = HUDBPClass.Class;



	InPool = false;

	Health = 10.0f;
	Shield = 0.0f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AHUDAndDamageCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHUDAndDamageCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHUDAndDamageCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AHUDAndDamageCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHUDAndDamageCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AHUDAndDamageCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AHUDAndDamageCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AHUDAndDamageCharacter::OnResetVR);

	// left click shooting functionality
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AHUDAndDamageCharacter::Shoot);
}

// Overlap Handling
// Breaks when pools are close enough to overlap simultaneously
// could be addressed using a stack, adding currently overlapped pools to stack
// each tick, if stack is not empty, use each pool in stack
void AHUDAndDamageCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->ActorHasTag("Pool")) {
		InPool = true;
		Pool = Cast<APool>(OtherActor);
	}
}

void AHUDAndDamageCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->ActorHasTag("Pool")) {
		InPool = false;
		Pool = nullptr;
	}
}

// Shoot Handling
void AHUDAndDamageCharacter::Shoot() {
	if (!MyController->bShowMouseCursor) {

		// test damage and display with random number values for damage
		TestDamage = FMath::RandRange(5, 25);

		// Line Trace from player camera to world projection of crosshair
		FHitResult Hit;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Trace")), true, this);
		GetWorld()->LineTraceSingleByChannel(Hit, MyController->PlayerCameraManager->GetCameraLocation(), MyController->PlayerCameraManager->GetCameraLocation() + MyController->PlayerCameraManager->GetActorForwardVector() * 10000, ECC_Visibility, TraceParams);
		//DrawDebugLine(GetWorld(), MyController->PlayerCameraManager->GetCameraLocation(), Hit.ImpactPoint, FColor::Red, true);

		// handle damage if hit actor is damagable
		if (Hit.GetActor()->Tags.Contains("Damagable")) {
			ADamagableObject* HitObject = Cast<ADamagableObject>(Hit.GetActor());
			HitObject->GetHit(TestDamage, Hit.ImpactPoint);
		}
	}
}

void AHUDAndDamageCharacter::OnResetVR() {
	// If HUDAndDamage is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in HUDAndDamage.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AHUDAndDamageCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location) {
		Jump();
}

void AHUDAndDamageCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location) {
		StopJumping();
}

void AHUDAndDamageCharacter::TurnAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHUDAndDamageCharacter::LookUpAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHUDAndDamageCharacter::MoveForward(float Value) {
	if ((Controller != nullptr) && (Value != 0.0f)) {
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AHUDAndDamageCharacter::MoveRight(float Value) {
	if ( (Controller != nullptr) && (Value != 0.0f) ) {
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AHUDAndDamageCharacter::TakeSomeDamage(float Damage) {
	float Overflow;
	
	// take damage from shield first, then health
	Overflow = FMath::Clamp(Damage - Shield, 0.0f,HEALTH_MAX + SHIELD_MAX);
	Shield = FMath::Clamp(Shield - Damage, 0.0f, SHIELD_MAX);
	Health = FMath::Clamp(Health - Overflow, 0.0f, HEALTH_MAX);
	
	// if health hits 0, restart level
	if (Health <= 0) {
		MyController->RestartLevel();
	}
}

void AHUDAndDamageCharacter::BeginPlay() {
	Super::BeginPlay();

	MyController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// HUD init and display
	wHUD = CreateWidget<UPlayerHUD>(GetWorld(), HUDClass);
	wHUD->AddToViewport();

	// init health and shield
	wHUD->HealthBar->Percent = Health;
	wHUD->ShieldBar->Percent = Shield;

}

void AHUDAndDamageCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// update legend based on controller rotation
	wHUD->OrientLegend(MyController->GetControlRotation().Yaw);
	
	// if standing in a pool 'Use' overlapped pool
	// see Overlap function for extended solution ~(line: 100)
	if (InPool) {
		Pool->Use(this);
	}
}