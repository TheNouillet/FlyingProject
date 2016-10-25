// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PrimaryWeapon.generated.h"

UCLASS()
class FLYINGPROJECT_API APrimaryWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrimaryWeapon();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/* Mesh properties */
	/** Attach the current weapon to the designated pawn (plane) on the "Primary_Weapon" socket */
	UFUNCTION(BlueprintCallable, Category = Mesh)
		bool AttachWeaponToPawn(class AFlyingProjectPawn* OwnerPawn);
	/** The visual mesh of the weapon */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly)
		class UStaticMeshComponent* WeaponMesh;
	/* The pawn (plane) than own the weapon */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly)
		class AFlyingProjectPawn* OuterPawn;

	/* Particle system */
	UPROPERTY(Category = Particle, VisibleDefaultsOnly, BlueprintReadOnly)
		class UParticleSystemComponent* ParticleSystemComponent;
	/*UPROPERTY(Category = Particle, VisibleDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<class UParticleSystemComponent> ParticleSystemComponentClass;*/

	/* Shooting mechanics */
	UPROPERTY(Category = Shooting, BlueprintReadOnly)
		class UPrimaryWeaponTypeComponent* WeaponTypeComponent;
	UPROPERTY(Category = Shooting, EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UPrimaryWeaponTypeComponent> WeaponTypeComponentClass;
	UFUNCTION(Category = Shooting, BlueprintCallable)
		bool IsFiring();
	UFUNCTION(BlueprintCallable, Category = Shooting)
		void StartFiringSequence();
	UFUNCTION(BlueprintCallable, Category = Shooting)
		void StopFiringSequence();
	UFUNCTION(BlueprintCallable, Category = Shooting)
		void Resupply();
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		int32 CurrentAmmoCount;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float CurrentConeOfFire;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float CurrentConeOfFireCooldown;
	UFUNCTION(BlueprintCallable, Category = Shooting)
		float ApplyConeOfFireBloom();
	UFUNCTION(BlueprintCallable, Category = Shooting)
		FVector GetCameraCenterLocation();
	UFUNCTION(BlueprintCallable, Category = Shooting)
		FRotator GetCameraCenterRotation();
	UFUNCTION(BlueprintCallable, Category = Shooting)
		FVector GetFireLocation();
	UFUNCTION(BlueprintCallable, Category = Shooting)
		FRotator GetFireRotation();
	UFUNCTION(BlueprintCallable, Category = Shooting)
		void TraceLine(FHitResult & HitResult, FRotator& OutAngle);
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		bool bTraceDebugLine;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		bool bTraceDebugImpact;

	/* Sounds */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		USoundWave* GunSound;

	/* Weapon stats */
	/** Damage of one bullet */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float Damage;
	/** Time between two shots */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float RefireTime;
	/** Maximum of ammunition that can be carried */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		int32 MaxAmmunition;
	/* Maximum distance at which the weapon can damage targets */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float WeaponRange;
	/* Maximum angle at which the hitscan can deviate from the center of the crosshair on the first shot */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float InitialConeOfFire;
	/* Maximum value at which the cone of fire can bloom after some shots */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float MaximumConeOfFire;
	/* Amount of angle that increase the cone of fire at each shot after the first one */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float ConeOfFireBloom;
	/* Amount of time to wait for the cone of fire to return to its initial value*/
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float ConeOfFireResetCooldown;
};
