// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DummyTarget.generated.h"

UCLASS()
class FLYINGPROJECT_API ADummyTarget : public APawn, public IDamageable, public ILockable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADummyTarget();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/* Implements IDamageable interface */
	virtual void Hurt_Implementation(float Damage);
	virtual float GetHealth_Implementation();
	virtual bool IsDead_Implementation();

	/* Implements ILockable interface */
	virtual bool IsLockable_Implementation();
	virtual FVector GetLockableLocation_Implementation();
	virtual void OnLock_Implementation();
	virtual void OnDelock_Implementation();

	/*UFUNCTION(BlueprintNativeEvent, BlueprintImplementableEvent, Category = "Lockable")
		void OnLock();
	UFUNCTION(BlueprintNativeEvent, BlueprintImplementableEvent, Category = "Lockable")
		void OnDelock();*/

	UStaticMeshComponent* SphereVisual;
	UPROPERTY(Category = Shooting, EditAnywhere, BlueprintReadWrite)
		float MaxHealth;
	UPROPERTY(Category = Shooting, EditAnywhere, BlueprintReadWrite)
		float Health;
};
