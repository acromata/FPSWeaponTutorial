#include "../Player/PlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);

	// Gun mesh
	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>("Gun Mesh");
	GunMesh->SetupAttachment(Camera);

	// Gun values
	GunRange = 500.f;
	BulletDamage = 1.f;
	MaxAmmo = 30;
	AmmoInMag = 90;
	bIsReloading = false;
	TimeBetweenShots = 50.f;
	bCanShoot = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set current ammo to max ammo
	CurrentAmmo = MaxAmmo;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Subtract time since last shot every frame (Better to do this using a timer)
	TimeSinceLastShot--;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add input mapping context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	// Input actions
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);

		Input->BindAction(ShootAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Shoot);
		Input->BindAction(ShootAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndShoot);
		Input->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Reload);
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (IsValid(Controller))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}

}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Jump()
{
	ACharacter::Jump();
}

void APlayerCharacter::Shoot()
{
	if (CurrentAmmo > 0 && !bIsReloading && TimeSinceLastShot <= 0 && bCanShoot)
	{
		// Line trace data
		FHitResult HitResult;
		FVector StartLocation = GunMesh->GetSocketLocation("Start");
		FVector EndLocation = StartLocation + (Camera->GetComponentRotation().Vector() * GunRange);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		
		// Subtract ammo
		CurrentAmmo--;

		// Time since last shot is now
		TimeSinceLastShot = TimeBetweenShots;
		
		// Check if automatic weapon, if not, stop firing after one shot
		if (!bIsAutomatic)
		{
			bCanShoot = false;
		}

		// Fire linetrace
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, CollisionParams);
		if (bHit)
		{
			AActor* ActorHit = HitResult.GetActor();
			if (IsValid(ActorHit))
			{
				ActorHit->Destroy();
			}
		}

		// Debug lines, not visible in Shipping builds.
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1, 0, 1);
	}
}

void APlayerCharacter::EndShoot()
{
	// Allow shooting again if not automatic
	if (!bIsAutomatic)
	{
		bCanShoot = true;
	}
}

void APlayerCharacter::Reload()
{
	if (CurrentAmmo != MaxAmmo) // Don't allow reload if you have max ammo
	{
		// Calculate the ammo in magazine so no ammo is lost during reload.
		if (AmmoInMag >= MaxAmmo)
		{
			// Get ammo to subtract from the magazine
			int32 AmmoToSubFromMag = MaxAmmo - CurrentAmmo;

			// Edit values
			CurrentAmmo = MaxAmmo;
			AmmoInMag -= AmmoToSubFromMag;
		}
		else if (AmmoInMag > 0)
		{
			int32 AmmoInGun = CurrentAmmo;
			CurrentAmmo = AmmoInMag;
			AmmoInMag = AmmoInGun;
		}

		//bIsReloading = true;
		// Call reload animation on weapon here
	}
}

void APlayerCharacter::AddAmmo(int32 AmmoToAdd)
{
	AmmoInMag += AmmoToAdd;
}

