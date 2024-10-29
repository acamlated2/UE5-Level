// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "UE5LevelCharacter.h"
#include "UE5LevelProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if (!isHoldingObject)
	{
		PickUpObject();
	}
	else
	{
		ThrowObject();
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

bool UTP_WeaponComponent::AttachWeapon(AUE5LevelCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
		}
	}

	return true;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

void UTP_WeaponComponent::PickUpObject()
{	
	FindRayPoints();

	HoldPoint = Cast<USceneComponent>(cameraComponent->GetChildComponent(3));

	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.AddIgnoredActor(Character);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, rayStart, rayEnd, ECC_Visibility, CollisionParams);

	if (bHit)
	{
		AActor* HitActor = Hit.GetActor();
		UPickableObject* PickableObjectComponent = Cast<UPickableObject>(HitActor->GetComponentByClass(UPickableObject::StaticClass()));
		if (!PickableObjectComponent)
		{                             
			return;
		}

		HeldObject = PickableObjectComponent->GetOwner();

		UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(HeldObject->GetRootComponent());
		StaticMeshComponent->SetSimulatePhysics(false);
		
		UPrimitiveComponent* HeldObjectRootComponent = Cast<UPrimitiveComponent>(HeldObject->GetRootComponent());
		
		if (HeldObjectRootComponent)
		{
			HeldObjectRootComponent->AttachToComponent(HoldPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		
			isHoldingObject = true;
		
			HeldObject->SetActorEnableCollision(false);
		}
	}
}

void UTP_WeaponComponent::ThrowObject()
{
	if (!HeldObject)
	{
		return;
	}

	UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(HeldObject->GetRootComponent());
	StaticMeshComponent->SetSimulatePhysics(true);

	HeldObject->SetActorEnableCollision(true);

	UPrimitiveComponent* HeldObjectRootComponent = Cast<UPrimitiveComponent>(HeldObject->GetRootComponent());

	UE::Math::TVector<double> CameraForward = cameraComponent->GetComponentTransform().GetRotation().GetForwardVector();
	HeldObjectRootComponent->AddForce(CameraForward * ThrowForce * ThrowForceMultiplier);

	isHoldingObject = false;
}

void UTP_WeaponComponent::FindRayPoints()
{
	cameraComponent = Character->GetFirstPersonCameraComponent();
	FTransform cameraTransform = cameraComponent->GetComponentTransform();

	rayStart = cameraTransform.GetLocation();

	rayEnd = (cameraTransform.GetRotation().GetForwardVector() * PickupMaxRange) + cameraTransform.GetLocation();
}
