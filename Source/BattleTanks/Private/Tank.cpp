// Fill out your copyright notice in the Description page of Project Settings.

#include "../BattleTanks/Public/Tank.h"
#include "../BattleTanks/Public/Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Engine/World.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<UStaticMeshComponent*> Components;
	GetComponents(Components);

	

	if (Components.Num() < 5)
	{
		UE_LOG(LogTemp, Error, TEXT("Tank has less than 5 members. Something is wrong."));
	}
	else
	{
		for (auto& Component : Components)
		{
			if (Component->GetName().Equals("Turret")) Turret = Component;
			else if (Component->GetName().Equals("Barrel")) Barrel = Component;
			else if (Component->GetName().Equals("LeftTrack")) LeftTrack = Component;
			else if (Component->GetName().Equals("RightTrack")) RightTrack = Component;
		}
	}
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATank::Fire()
{
	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTime;

	if (Barrel && isReloaded)
	{
		LastFireTime = FPlatformTime::Seconds();

		auto projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Muzzle")),
			Barrel->GetSocketRotation(FName("Muzzle"))
			);

		projectile->LaunchProjectile(LaunchSpeed);
	}
}

void ATank::AimAt(FVector AimLocation)
{
	if (!Barrel || !Turret) return;

	FVector LaunchDirection;
	FVector startLoc = Barrel->GetSocketLocation(FName("Muzzle"));
	if (UGameplayStatics::SuggestProjectileVelocity(
			this,
			LaunchDirection,
			startLoc,
			AimLocation,
			LaunchSpeed,
			false,
			0,
			0,
			ESuggestProjVelocityTraceOption::DoNotTrace)
		)
	{
		FRotator AimAsRotator          = LaunchDirection.GetSafeNormal().Rotation();
		FRotator currentBarrelRotation = Barrel->GetForwardVector().Rotation();
		FRotator currentTurretRotation = Turret->GetForwardVector().Rotation();

		ElevateBarrel(FMath::Clamp(
			AimAsRotator.Pitch - currentBarrelRotation.Pitch,
			barrelDegreesPerSecond * -1,
			barrelDegreesPerSecond));

		TurnTurret(FMath::Clamp(
			AimAsRotator.Yaw - currentTurretRotation.Yaw,
			turretDegreesPerSecond * -1,
			turretDegreesPerSecond));

		//rrel->AddLocalRotation(FRotator(PitchDifference, 0, 0));

		//DrawDebugLine(GetWorld(), startLoc, startLoc + LaunchDirection * 10000, FColor::Red);
	}
}

void ATank::ElevateBarrel(float speed)
{
	auto ElevationChange = speed * barrelDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto newElevation = FMath::Clamp(
		Barrel->RelativeRotation.Pitch + ElevationChange, 
		barrelMinElevation, 
		barrelMaxElevation);

	Barrel->SetRelativeRotation(FRotator(newElevation, 0, 0));
}

void ATank::TurnTurret(float speed)
{
	auto AzimuthChange = speed * turretDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto newAzimuth = Turret->RelativeRotation.Yaw + AzimuthChange;
	Turret->SetRelativeRotation(FRotator(0, newAzimuth, 0));
}

