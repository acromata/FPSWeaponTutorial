#include "../Misc/Pickup.h"
#include "../Player/PlayerCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>("Collider");
	Collider->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Collider);

	AmmoToAdd = 10;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	Collider->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapBegin);
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called on overlap
void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if player overlapped
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (IsValid(Player))
	{
		// Add ammo to player mag
		Player->AddAmmo(AmmoToAdd);
		Destroy();
	}
}

