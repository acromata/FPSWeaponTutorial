#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class FPSSYSTEM_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()


protected:
	// Components
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* GunMesh;

	// Mappings
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* InputMapping;

	// Actions
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* ReloadAction;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// Movement
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();

	// Shooting
	void Shoot();
	void EndShoot();
	void Reload();

	UPROPERTY(EditAnywhere, Category = "Gun")
	float GunRange;
	UPROPERTY(EditAnywhere, Category = "Gun")
	float BulletDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun")
	int32 MaxAmmo;
	UPROPERTY(BlueprintReadOnly, Category = "Gun")
	int32 CurrentAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun")
	int32 AmmoInMag;
	UPROPERTY(BlueprintReadWrite, Category = "Gun")
	bool bIsReloading;
	UPROPERTY(EditAnywhere, Category = "Gun")
	float TimeBetweenShots;
	float TimeSinceLastShot;
	UPROPERTY(EditAnywhere, Category = "Gun")
	bool bIsAutomatic;

	bool bCanShoot;

public:

	void AddAmmo(int32 AmmoToAdd);

};
