// Fill out your copyright notice in the Description page of Project Settings.

#include "Ball.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Board.h"

// Sets default values
ABall::ABall()
{
	/* sphere collision  */
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereCollision);
	SphereCollision->SetSphereRadius(24.f);

	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereCollision->SetCollisionProfileName("BlockAll");

	/**  paper sprite component */
	BallSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Paper Sprite Component"));
	BallSpriteComponent->SetupAttachment(RootComponent);
	BallSpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/** set ball sprite asset */
	static ConstructorHelpers::FObjectFinder<UPaperSprite> PaperSpriteTemplate(TEXT("/Game/Assets/Sprites/Ball_Sprite"));
	if (PaperSpriteTemplate.Succeeded())
	{
		BallSpriteComponent->SetSprite(PaperSpriteTemplate.Object);
	}

	BallMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	BallMovementComponent->ProjectileGravityScale = 0.f;
	BallMovementComponent->InitialSpeed = 1000.f;
	BallMovementComponent->MaxSpeed = 10000.f;
	BallMovementComponent->bShouldBounce = true;
	BallMovementComponent->Bounciness = 1.1;
	BallMovementComponent->Friction = 0.f;
	BallMovementComponent->SetIsReplicated(true);

	InitialLifeSpan = 20.f;

	// Replication
	SetReplicates(true);
	SetReplicateMovement(true);
}

void ABall::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (Role == ROLE_Authority)
	{
		if (ABoard* Board = Cast<ABoard>(GetOwner()))
		{
			Board->SetCanSpawn(true);
			Board->NillifyBallRef();
		}
	}
}