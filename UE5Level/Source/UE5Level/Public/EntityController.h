// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EntityController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5LEVEL_API UEntityController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEntityController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 100;

	void DamageEntity(float Damage);

	UFUNCTION(BlueprintCallable)
	void SetOnFire(bool OnFire, float Damage, float DamageInterval);

protected:
	void Die();

	bool IsOnFire = false;
	float FireDamage = 10;
	float FireDamageInterval = 0.5;
	float FireDamageIntervalTimer;
	bool FireVulnerable = true;
};
