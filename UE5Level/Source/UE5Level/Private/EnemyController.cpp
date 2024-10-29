// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

#include "EntityController.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UEnemyController::UEnemyController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemyController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ShouldShoot)
	{
		ShootFrequencyTimer -= 1* DeltaTime;
		if (ShootFrequencyTimer <= 0)
		{
			ShootFrequencyTimer = ShootFrequency;

			Shoot();
		}
	}
}

void UEnemyController::TrackPlayer(AActor* PlayerCharacter)
{
	Character = PlayerCharacter;
	
	RayStart = GetOwner()->GetTransform().GetLocation();
	RayEnd = Character->GetTransform().GetLocation();

	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, RayStart, RayEnd, ECC_Visibility, CollisionParams);

	if (bHit)
	{
		AActor* HitActor = Hit.GetActor();

		UCameraComponent* CameraComponent = Cast<UCameraComponent>(HitActor->GetComponentByClass(UCameraComponent::StaticClass()));
		if (!CameraComponent)
		{
			ShouldShoot = false;
			ShootFrequencyTimer = ShootFrequency;
			return;
		}

		ShouldShoot = true;
		return;
	}

	ShouldShoot = false;
	ShootFrequencyTimer = ShootFrequency;
}

void UEnemyController::Shoot()
{
	UEntityController* EntityController = Cast<UEntityController>(Character->GetComponentByClass(UEntityController::StaticClass()));

	EntityController->DamageEntity(Damage);
}

