// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5LEVEL_API UEnemyController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootFrequency = 1;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void TrackPlayer(AActor* PlayerCharacter);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 20;

private:
	bool ShouldShoot = false;

	UE::Math::TVector<double> RayStart;
	UE::Math::TVector<double> RayEnd;

	float ShootFrequencyTimer = 1;

	void Shoot();

	AActor* Character;
};
