// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingProject.h"
#include "PrimaryWeapon.h"
#include "Damageable.h"
#include "PrimaryWeaponTypeComponent.h"


// Sets default values for this component's properties
UPrimaryWeaponTypeComponent::UPrimaryWeaponTypeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	bIsFiring = false;
	refireCooldown = 0.f;
}


// Called when the game starts
void UPrimaryWeaponTypeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPrimaryWeaponTypeComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	Shoot();
}

bool UPrimaryWeaponTypeComponent::ShootProjectile(FRotator& OutAngle, FVector& OutImpactLocation)
{
	FHitResult HitResult;
	if (OuterWeapon)
	{
		OuterWeapon->TraceLine(HitResult, OutAngle);
		if (OuterWeapon->ParticleSystemComponent != nullptr)
		{
			//OuterWeapon->ParticleSystemComponent->ActivateSystem(true);
			//OuterWeapon->ParticleSystemComponent->ToggleActive();
		}
		if (HitResult.GetActor())
		{
			IDamageable* damageableActor = Cast<IDamageable>(HitResult.GetActor());
			if (damageableActor)
			{
				damageableActor->Execute_Hurt(HitResult.GetActor(), OuterWeapon->Damage);
			}
			OutImpactLocation = HitResult.ImpactPoint;
			return true;
		}
	}

	return false;
}

bool UPrimaryWeaponTypeComponent::IsFiring()
{
	return bIsFiring;
}

float UPrimaryWeaponTypeComponent::GetRefireCooldown()
{
	return refireCooldown;
}

