// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "PrimaryWeaponTypeComponent.generated.h"


UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLYINGPROJECT_API UPrimaryWeaponTypeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPrimaryWeaponTypeComponent();
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Shooting")
		void StartFiringSequence();
	UFUNCTION(BlueprintImplementableEvent, Category = "Shooting")
		void Shoot();
	UFUNCTION(BlueprintCallable, Category = "Shooting")
		bool ShootProjectile(FRotator& OutAngle, FVector& OutImpactLocation);
	UFUNCTION(BlueprintImplementableEvent, Category = "Shooting")
		void StopFiringSequence();
	bool IsFiring();
	float GetRefireCooldown();
	UPROPERTY(Category = Shooting, BlueprintReadWrite)
		class APrimaryWeapon* OuterWeapon;

protected:
	UPROPERTY(Category = Shooting, BlueprintReadWrite)
		bool bIsFiring;
	UPROPERTY(Category = Shooting, BlueprintReadWrite)
		float refireCooldown;
};
