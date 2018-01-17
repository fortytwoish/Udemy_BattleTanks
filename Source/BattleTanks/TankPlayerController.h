// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BattleTanks/Public/Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANKS_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public: 
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float ReticleXLocation = 0.5f;

	UPROPERTY(EditAnywhere)
	float ReticleYLocation = 0.3f;

	UPROPERTY(EditAnywhere)
	float LineTraceRange = 100000.f;

private:
	ATank* GetControlledTank() const;
	//Move barrel towards crosshair so that shots hit
	//world at crosshair intersection when rotation finishes
	void AimTowardsCrosshair();
	bool GetSightRayHitLocation(FVector& HitLocation) const;
	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;
};
