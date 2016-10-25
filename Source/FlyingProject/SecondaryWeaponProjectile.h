// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "SecondaryWeaponProjectile.generated.h"

UCLASS()
class FLYINGPROJECT_API ASecondaryWeaponProjectile : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecondaryWeaponProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called when the projectile collides something
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay")
		void TrackTarget(float DeltaTime);

public:

	// Called whenever the projectile have to explode
	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay")
		void Explode();
	// Called whenever we activate the homing-to-target feature of the projectile
	//UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay")
		//void ActivateHoming();

	//Visual
	/** Actual representation of the projectile */
	UPROPERTY(Category = Visual, VisibleDefaultsOnly, BlueprintReadOnly)
		class UStaticMeshComponent* ProjectileMesh;
	/** Projectile movement component that supports the homing-to-target system*/
	//UPROPERTY(Category = Visual, VisibleDefaultsOnly, BlueprintReadOnly)
		//UProjectileMovementComponent* ProjectileMovementComponent;
	/** Projectile trail particle system */
	UPROPERTY(Category = Visual, VisibleDefaultsOnly, BlueprintReadOnly)
		class UParticleSystemComponent* ParticleSystemComponent;

	/* Sounds */
	UPROPERTY(Category = Sounds, EditAnywhere, BlueprintReadWrite)
		USoundWave* ImpactSound;

	//Gameplay
	/** Damage dealt on hit */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float Damage;
	/** Speed of the projectile */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float Speed;
	/** Turn rate of the projectile */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float TurnRate;
	/** Distance at which the projectile can travel until destruction */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float MaxLifeTime;
	/** Amount of time before the projectile head to the target */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float TimeBeforeTracking;
	/** Distance from the target at which the projectile explode*/
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float Radius;
	/** Target the projectile have to head to */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		AActor* Target;
	/** Minimmum angle at which the projectile consider being "on" the target */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float MinAngle;

protected:
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float LifeTime;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		FVector MoveVector;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		FVector Direction;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		bool bHasExploded;
};
