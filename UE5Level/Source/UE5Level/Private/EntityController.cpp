// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityController.h"

// Sets default values for this component's properties
UEntityController::UEntityController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UEntityController::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UEntityController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsOnFire)
	{
		if (FireVulnerable)
		{
			DamageEntity(FireDamage);
		}

		FireVulnerable = false;
		FireDamageIntervalTimer -= 1 * DeltaTime;
		if (FireDamageIntervalTimer <= 0)
		{
			FireDamageIntervalTimer = FireDamageInterval;

			FireVulnerable = true;
		}
	}
}

void UEntityController::DamageEntity(float Damage)
{
	Health -= Damage;

	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow,
		FString::Printf(TEXT("Current Health: %f"), Health));

	if (Health <= 0)
	{
		Die();
	}
}

void UEntityController::SetOnFire(bool OnFire, float Damage, float DamageInterval)
{
	IsOnFire = OnFire;
	FireDamage = Damage;
	FireDamageInterval = DamageInterval;
	FireDamageIntervalTimer = DamageInterval;

	if (!OnFire)
	{
		FireVulnerable = true;
		FireDamageIntervalTimer = DamageInterval;
	}
}

void UEntityController::Die()
{	
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
			FString::Printf(TEXT("%s Ran out of health"), *GetOwner()->GetName()));
	
	GetOwner()->Destroy();
}

