// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Pawn.h"
#include "FlyingProjectPawn.generated.h"

UCLASS(config=Game)
class AFlyingProjectPawn : public APawn
{
	GENERATED_BODY()


	/** StaticMesh component that will be the visuals for our flying pawn */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PlaneMesh;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	//class ILockable* Target;

public:
	AFlyingProjectPawn();

	// Begin AActor overrides
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End AActor overrides

protected:

	// Begin APawn overrides
	//virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override; // Allows binding actions/axes to functions
	// End APawn overrides

	/** Bound to the thrust axis */
	void ThrustInput(float Val);
	
	/** Bound to the vertical axis */
	void MoveUpInput(float Val);

	/** Bound to the horizontal axis */
	void MoveRightInput(float Val);

private:

	/** How quickly forward speed changes */
	UPROPERTY(Category=Plane, EditAnywhere)
	float Acceleration;

	/** How quickly pawn can steer */
	UPROPERTY(Category=Plane, EditAnywhere)
	float TurnSpeed;

	/** Max forward speed */
	UPROPERTY(Category = Pitch, EditAnywhere)
	float MaxSpeed;

	/** Min forward speed */
	UPROPERTY(Category=Yaw, EditAnywhere)
	float MinSpeed;

	/** Distance at which the camera foolow the Pawn */
	UPROPERTY(Category = Camera, EditAnywhere)
	float CameraArmLength;

	/** Current forward speed */
	float CurrentForwardSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;

public:
	/** Returns PlaneMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
	/** Targeting component that will be used to lock targetable actors*/
	UPROPERTY(Category = Targeting, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UTargetingComponent* TargetingComponent;

	/** Command the Thrust of the vehicle*/
	void CommandThrustInput(float val);
	void CommandMoveUp(float val);
	void CommandMoveRight(float val);

	/* Shooting mechanics */
	void CommandShootPrimary();
	void CommandStopPrimary();
	void CommandShootSecondary();
	void CommandStopSecondary();

	/* Networking */
	// Send transform infos to server to broadcast for other clients.
	UFUNCTION(Server, Reliable, WithValidation, Category = "Networking")
	void Server_SendLocationAndRotation(const FVector& NewLocation, const FRotator& NewRotation);
	virtual void Server_SendLocationAndRotation_Implementation(const FVector& NewLocation, const FRotator& NewRotation);
	virtual bool Server_SendLocationAndRotation_Validate(const FVector& NewLocation, const FRotator& NewRotation);
	FVector PreviousLocation;

	/* Locking mechanics */
	// Targeting Component class to use
	UPROPERTY(Category = Targeting, EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UTargetingComponent> TargetingComponentClass;
	void CommandLock();
	void Lock(class ILockable* ProvidedTarget);

	/** Is rear view toggled*/
	UPROPERTY(Category = Camera, EditAnywhere, BlueprintReadWrite)
	bool bIsRearCameraActivated;
	void CommandRearView();
	void CommandNormalView();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	float GetSpeed();

	UFUNCTION(BlueprintCallable, Category = "Mesh")
		class UStaticMeshComponent* GetPlaneMesh();
	UFUNCTION(BlueprintCallable, Category = "Mesh")
		void GetSecondaryProjectileSpawn(FVector& OutLocation, FRotator& OutRotation);
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite)
		bool bSecondarySpawnParity;
	UFUNCTION(BlueprintImplementableEvent, Category = "Mesh")
		void OnCreatePrimaryWeapon();
	UFUNCTION(BlueprintImplementableEvent, Category = "Mesh")
		void OnCreateSecondaryWeapon();

	UPROPERTY(Category = Mesh, BlueprintReadWrite)
	class APrimaryWeapon *PrimaryWeapon;
	UPROPERTY(Category = Mesh, BlueprintReadWrite)
	class ASecondaryWeapon *SecondaryWeapon;
};
