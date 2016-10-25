// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "FlyingProjectPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FLYINGPROJECT_API AFlyingProjectPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AFlyingProjectPlayerController();
	~AFlyingProjectPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent();
	virtual void Tick(float delta) override;

protected:
	void generateRatiosFromMouseInput();

public:
	//Axis commands
	void CommandThrustUp(float val);
	void CommandMoveUp(float val);
	void CommandMoveUpFromMouseInput(float val);
	void CommandMoveRight(float val);
	void CommandMoveRightFromMouseInput(float val);
	
	//Action commands
	void CommandShootPrimary();
	void CommandStopPrimary();
	void CommandShootSecondary();
	void CommandStopSecondary();
	void CommandRearView();
	void CommandNormalView();
	void CommandPause();
	void CommandLock();

	/** Y-Axis is inverted ?*/
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	bool bIsYAxisInverted;
	/** Is mouse input enabled ?*/
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	bool bIsMouseInputEnabled;
	/** Dead zone size for mouse input mode */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MinimumBoundaryPercentage;
	/** Maximume input zone size for mouse input mode on the X axis*/
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MaximumXBoundaryPercentage;
	/** Maximume input zone size for mouse input mode on the Y axis*/
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MaximumYBoundaryPercentage;

	/* Blueprint implementable event for pausing the game*/
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void PauseEvent();

	/** Debug properties + meaningless blueprint functions*/
	UPROPERTY(Category = Debug, EditAnywhere, BlueprintReadOnly)
	float xRatio;
	UPROPERTY(Category = Debug, EditAnywhere, BlueprintReadOnly)
	float yRatio;
	UPROPERTY(Category = Debug, EditAnywhere, BlueprintReadOnly)
	float xActualRatio;
	UPROPERTY(Category = Debug, EditAnywhere, BlueprintReadOnly)
	float yActualRatio;
	UPROPERTY(Category = Debug, EditAnywhere, BlueprintReadOnly)
	float xMousePosition;
	UPROPERTY(Category = Debug, EditAnywhere, BlueprintReadOnly)
	float yMousePosition;
	UPROPERTY(Category = Debug, EditAnywhere, BlueprintReadOnly)
	bool bIsMouseInBoundary;
	UPROPERTY(Category = Debug, EditAnywhere, BlueprintReadWrite)
	bool bIsBlueprintDerived;
};
