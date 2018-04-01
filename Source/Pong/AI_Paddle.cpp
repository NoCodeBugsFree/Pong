// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_Paddle.h"
#include "Ball.h"

void AAI_Paddle::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
}

void AAI_Paddle::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/** if we have an active ball adjust out location to ball Y location  */
	if (Ball)
	{
		FVector BallLocation = Ball->GetActorLocation();

		FVector TargetLocation = GetActorLocation();

		TargetLocation.Y = BallLocation.Y;

		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, DeltaSeconds, 600.f));
	}
	else /** go to initial location */
	{
		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), InitialLocation, DeltaSeconds, 600.f));
	}
}
