#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class FPSSYSTEM_API APickup : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* Collider;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Values")
	int32 AmmoToAdd;

};
