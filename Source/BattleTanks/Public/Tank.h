// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class AProjectile;

UCLASS()
class BATTLETANKS_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Fire();

	void AimAt(FVector HitLocation);
	
	UPROPERTY(EditAnywhere, Category = Firing)
	TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditAnywhere, Category = Firing)
	float LaunchSpeed = 100000.f;

	UPROPERTY(EditAnywhere, Category = Firing)
	float ReloadTime = 3.f;

	UPROPERTY(EditAnywhere, Category = Firing)
	float barrelMinElevation = 0.f;

	UPROPERTY(EditAnywhere, Category = Firing)
	float barrelMaxElevation = 40.f;

	UPROPERTY(EditAnywhere, Category = Firing)
	float barrelDegreesPerSecond = 10.f;

	UPROPERTY(EditAnywhere, Category = Firing)
	float turretDegreesPerSecond = 10.f;

private:
	UStaticMeshComponent* Barrel = nullptr;
	UStaticMeshComponent* LeftTrack = nullptr;
	UStaticMeshComponent* RightTrack = nullptr;
	UStaticMeshComponent* Turret = nullptr;

	void ElevateBarrel(float speed);
	void TurnTurret(float speed);
	double LastFireTime = 0;
};
