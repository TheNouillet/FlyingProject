// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SecondaryWeapon.generated.h"

UCLASS()
class FLYINGPROJECT_API ASecondaryWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecondaryWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/* Mesh properties */
	/** Attach the current weapon to the designated pawn (plane) on the "Secondary_Weapon" sockets */
	UFUNCTION(BlueprintCallable, Category = Visual)
		bool AttachWeaponToPawn(class AFlyingProjectPawn* OwnerPawn);
	/** The visual mesh of the weapon */
	UPROPERTY(Category = Visual, VisibleDefaultsOnly, BlueprintReadOnly)
		class UStaticMeshComponent* WeaponMesh1;
		/** The visual mesh of the weapon (2)*/
	UPROPERTY(Category = Visual, VisibleDefaultsOnly, BlueprintReadOnly)
		class UStaticMeshComponent* WeaponMesh2;
	/* The pawn (plane) than own the weapon */
	UPROPERTY(Category = Visual, VisibleDefaultsOnly, BlueprintReadOnly)
		class AFlyingProjectPawn* OuterPawn;
	/** Class of the projectile to use */
	UPROPERTY(Category = Visual, EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class ASecondaryWeaponProjectile> ProjectileClass;
	
	/* Shooting mechanics */
	UPROPERTY(Category = Shooting, BlueprintReadOnly)
		class USecondaryWeaponTypeComponent* WeaponTypeComponent;
	UPROPERTY(Category = Shooting, EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class USecondaryWeaponTypeComponent> WeaponTypeComponentClass;
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

	/* Sounds */
	UPROPERTY(Category = Sounds, EditAnywhere, BlueprintReadWrite)
		USoundWave* SpawnSound;
	UPROPERTY(Category = Sounds, EditAnywhere, BlueprintReadWrite)
		USoundWave* ImpactSound;

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
	/** Number of missiles volleys per shot */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		int32 NumberOfVolleys;
	/** Number of missiles per volleys */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		int32 MissilesPerVolley;
	/** Amount of time between two volleys*/
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float DelayBetweenVolleys;
	/** Speed at which the missile travel*/
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float Speed;
	/** Turn rate of a projectile when heading toward a locked target */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float TurnRate;
	/** Distance at which a target is locked on */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float LockRange;
	/** Angle at which a target is locked on */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float LockAngle;
	/** Maximum distance a projectile can travel before being destroyed */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float MaxLifeTime;
	/** Amount of time before the projectile head to the target*/
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float TimeBeforeTracking;
	/** Distance of the target at which the projectile consider hitting*/
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float Radius;
};
