// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingProject.h"
#include "FlyingProjectPawn.h"
#include "FlyingProjectPlayerController.h"

AFlyingProjectPlayerController::AFlyingProjectPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	InputComponent = CreateDefaultSubobject<UInputComponent>(TEXT("InputComponent"));

	bShowMouseCursor = true;
	/*bEnableClickEvents = true;
	bEnableMouseOverEvents = true;*/

	/*Configure default values*/
	bIsYAxisInverted = true;
	bIsMouseInputEnabled = true;
	MinimumBoundaryPercentage = 0.05f;
	MaximumYBoundaryPercentage = 0.80f;
	MaximumXBoundaryPercentage = 0.80f;
	bIsBlueprintDerived = false;
}

AFlyingProjectPlayerController::~AFlyingProjectPlayerController()
{
}

void AFlyingProjectPlayerController::Tick(float delta)
{
	if (bIsMouseInputEnabled)
	{
		generateRatiosFromMouseInput();
	}
}

void AFlyingProjectPlayerController::generateRatiosFromMouseInput()
{
	//Get mouse cursor position
	float x;
	float y;
	bIsMouseInBoundary = GetMousePosition(x, y);
	//Public information to display on debug hud (yeah i know, i'm a lazy guy :p)
	xMousePosition = x;
	yMousePosition = y;

	//If mouse position is in window boundaries
	if(bIsMouseInBoundary)
	{
		//Get screen size + center coordinates
		int32 width;
		int32 height;
		GetViewportSize(width, height);
		int32 centerX = width / 2;
		int32 centerY = height / 2;

		//Old method
		/*int32 widthOffset = (width * 15) / 100;
		int32 heightOffset = (height * 15) / 100;
		xRatio = ((x * 2) / width) - 1.f;
		yRatio = ((y * 2) / height) - 1.f;*/

		//Process the boundaries for user comfort
		float minimumBoundary = ((float)centerY) * MinimumBoundaryPercentage;
		//Get maximum boundary for 2 X axis input
		float maximumBoundaryX = ((float)centerX) * MaximumXBoundaryPercentage;
		float maximumBoundaryY = ((float)centerY) * MaximumYBoundaryPercentage;

		//Process distance of mouse cursor from center of screen
		float distX = FGenericPlatformMath::Abs(x - ((float)centerX));
		float distY = FGenericPlatformMath::Abs(y - ((float)centerY));

		//Process the ratios
		//X Ratio
		if (distX <= minimumBoundary)
		{
			xRatio = 0.f;
		}
		else if (distX > maximumBoundaryX)
		{
			if (x > centerX)
			{
				xRatio = 2.0f;
			}
			else
			{
				xRatio = -2.0f;
			}
		}
		else
		{
			xRatio = ((distX - minimumBoundary) * 2) / (maximumBoundaryX - minimumBoundary);
			if (x < centerX)
			{
				xRatio *= -1.f;
			}
		}
		//Y Ratio
		if (distY <= minimumBoundary)
		{
			yRatio = 0.f;
		}
		else if (distY > maximumBoundaryY)
		{
			if (y > centerY)
			{
				yRatio = 2.0f;
			}
			else
			{
				yRatio = -2.0f;
			}
		}
		else
		{
			yRatio = ((distY - minimumBoundary) * 2) / (maximumBoundaryY - minimumBoundary);
			if (y < centerY)
			{
				yRatio *= -1.f;
			}
		}


		if (yRatio >= -2.f && yRatio <= 2.f)
		{
			CommandMoveUpFromMouseInput(yRatio * -1.f);
		}
		if (xRatio >= -2.f && xRatio <= 2.f)
		{
			CommandMoveRightFromMouseInput(xRatio);
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("%f"), x);
}

void AFlyingProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(this->GetPawn()))
	{
		Possess(this->GetPawn());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Possess isn't working"));
	}
}

void AFlyingProjectPlayerController::SetupInputComponent()
{
	//Check if the controller has an Input Component
	check(InputComponent);

	// Bind our control axis' to callback functions
	InputComponent->BindAxis("Thrust", this, &AFlyingProjectPlayerController::CommandThrustUp);
	InputComponent->BindAxis("MoveUp", this, &AFlyingProjectPlayerController::CommandMoveUp);
	InputComponent->BindAxis("MoveRight", this, &AFlyingProjectPlayerController::CommandMoveRight);

	//Action controls
	InputComponent->BindAction("ShootPrimary", IE_Pressed, this, &AFlyingProjectPlayerController::CommandShootPrimary);
	InputComponent->BindAction("ShootPrimary", IE_Released, this, &AFlyingProjectPlayerController::CommandStopPrimary);
	InputComponent->BindAction("ShootSecondary", IE_Pressed, this, &AFlyingProjectPlayerController::CommandShootSecondary);
	InputComponent->BindAction("ShootSecondary", IE_Released, this, &AFlyingProjectPlayerController::CommandStopSecondary);
	InputComponent->BindAction("RearView", IE_Pressed, this, &AFlyingProjectPlayerController::CommandRearView);
	InputComponent->BindAction("RearView", IE_Released, this, &AFlyingProjectPlayerController::CommandNormalView);
	InputComponent->BindAction("Pause", IE_Pressed, this, &AFlyingProjectPlayerController::CommandPause);
	InputComponent->BindAction("Lock", IE_Pressed, this, &AFlyingProjectPlayerController::CommandLock);
}

void AFlyingProjectPlayerController::CommandThrustUp(float val)
{
	auto ControlledCharacter = Cast<AFlyingProjectPawn>(this->GetPawn());
	if (ControlledCharacter != NULL)
	{
		ControlledCharacter->CommandThrustInput(val);
	}
}

void AFlyingProjectPlayerController::CommandMoveUp(float val)
{
	if (!bIsMouseInBoundary)
	{
		auto ControlledCharacter = Cast<AFlyingProjectPawn>(this->GetPawn());
		if (ControlledCharacter != NULL)
		{
			if (!bIsYAxisInverted)
			{
				ControlledCharacter->CommandMoveUp(val);
				yActualRatio = val;
			}
			else
			{
				ControlledCharacter->CommandMoveUp(val * -1.f);
				yActualRatio = val * -1.f;
			}
		}
	}
}

void AFlyingProjectPlayerController::CommandMoveUpFromMouseInput(float val)
{
	auto ControlledCharacter = Cast<AFlyingProjectPawn>(this->GetPawn());
	if (ControlledCharacter != NULL)
	{
		if (!bIsYAxisInverted)
		{
			ControlledCharacter->CommandMoveUp(val);
			yActualRatio = val;
		}
		else
		{
			ControlledCharacter->CommandMoveUp(val * -1.f);
			yActualRatio = val * -1.f;
		}
	}
}

void AFlyingProjectPlayerController::CommandMoveRight(float val)
{
	if (!bIsMouseInBoundary)
	{
		auto ControlledCharacter = Cast<AFlyingProjectPawn>(this->GetPawn());
		if (ControlledCharacter != NULL)
		{
			ControlledCharacter->CommandMoveRight(val);
			xActualRatio = val;
		}
	}
}

void AFlyingProjectPlayerController::CommandMoveRightFromMouseInput(float val)
{
	auto ControlledCharacter = Cast<AFlyingProjectPawn>(this->GetPawn());
	if (ControlledCharacter != NULL)
	{
		ControlledCharacter->CommandMoveRight(val);
		xActualRatio = val;
	}
}

void AFlyingProjectPlayerController::CommandShootPrimary()
{
	auto ControlledCharacter = Cast<AFlyingProjectPawn>(this->GetPawn());
	if (ControlledCharacter != NULL)
	{
		ControlledCharacter->CommandShootPrimary();
	}
}

void AFlyingProjectPlayerController::CommandStopPrimary()
{
	auto ControlledCharacter = Cast<AFlyingProjectPawn>(this->GetPawn());
	if (ControlledCharacter != NULL)
	{
		ControlledCharacter->CommandStopPrimary();
	}
}

void AFlyingProjectPlayerController::CommandShootSecondary()
{
	auto ControlledCharacter = Cast<AFlyingProjectPawn>(this->GetPawn());
	if (ControlledCharacter != NULL)
	{
		ControlledCharacter->CommandShootSecondary();
	}
}

void AFlyingProjectPlayerController::CommandStopSecondary()
{
	auto ControlledCharacter = Cast<AFlyingProjectPawn>(this->GetPawn());
	if (ControlledCharacter != NULL)
	{
		ControlledCharacter->CommandStopSecondary();
	}
}

void AFlyingProjectPlayerController::CommandRearView()
{
	auto ControlledCharacter = Cast<AFlyingProjectPawn>(this->GetPawn());
	if (ControlledCharacter != NULL)
	{
		ControlledCharacter->CommandRearView();
	}
}

void AFlyingProjectPlayerController::CommandNormalView()
{
	auto ControlledCharacter = Cast<AFlyingProjectPawn>(this->GetPawn());
	if (ControlledCharacter != NULL)
	{
		ControlledCharacter->CommandNormalView();
	}
}

void AFlyingProjectPlayerController::CommandPause()
{
	if (!bIsBlueprintDerived)
	{
		UE_LOG(LogTemp, Warning, TEXT("FlyingProjectPlayerController"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MyFlyingProjectPlayerController"));
		PauseEvent();
	}
}

void AFlyingProjectPlayerController::CommandLock()
{
	auto ControlledCharacter = Cast<AFlyingProjectPawn>(this->GetPawn());
	if (ControlledCharacter != NULL)
	{
		ControlledCharacter->CommandLock();
	}
}
