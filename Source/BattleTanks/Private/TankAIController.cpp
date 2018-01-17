// Fill out your copyright notice in the Description page of Project Settings.

#include "../BattleTanks/Public/TankAIController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "../Public/TankAIController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	auto myTank = GetControlledTank();
	if (!myTank)
	{
		UE_LOG(LogTemp, Error, TEXT("AI Controller doesn't control a Tank!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller controls Tank %s."), *(myTank->GetName()));
	}

	auto plTank = GetPlayerTank();
	if (!plTank)
	{
		UE_LOG(LogTemp, Error, TEXT("AI Controller hasn't found player tank!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller found player Tank: %s."), *(plTank->GetName()));
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto playerTank = GetPlayerTank();
	if (playerTank)
	{
		GetControlledTank()->AimAt(playerTank->GetActorLocation());
		GetControlledTank()->Fire();
	}
}

ATank * ATankAIController::GetPlayerTank() const
{
	return Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}
