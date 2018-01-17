// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ATank* tank = GetControlledTank();
	if (!tank)
	{
		UE_LOG(LogTemp, Error, TEXT("Tank Player Controller controls no tank!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tank Player Controller controls Tank %s."), *(tank->GetName()));
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	///Workaround for Tick-In-Editor bug
	if (GetWorld()->WorldType == EWorldType::EditorPreview) { return; }

	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	auto myTank = GetControlledTank();
	if (!myTank)
	{
		return;
	}

	FVector HitLocation;
	if (GetSightRayHitLocation(HitLocation))
	{
		GetControlledTank()->AimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{

	//Get Reticle Location
	int32 ViewportWidth, ViewportHeight;
	GetViewportSize(ViewportWidth, ViewportHeight);

	FVector2D Reticle2DLocation = FVector2D(ViewportWidth  * ReticleXLocation, 
										    ViewportHeight * ReticleYLocation);
	
	//De-Project Reticle to World Direction
	FVector LookDirection;
	if (GetLookDirection(Reticle2DLocation, LookDirection))
	{
		//Perform the Line Trace
		FHitResult HitResult;
		if (GetHitResultAtScreenPosition(Reticle2DLocation, ECollisionChannel::ECC_WorldStatic, false, HitResult))
		{
			HitLocation = HitResult.Location;
			return true;
		}
		HitLocation = FVector(1,1,1);
	}

	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector worldLoc;

	return DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		worldLoc,
		LookDirection
	);
}