// Fill out your copyright notice in the Description page of Project Settings.

#include "Paddle.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "ConstructorHelpers.h"
#include "PaperFlipbook.h"
#include "Components/CapsuleComponent.h"

APaddle::APaddle()
{
	/* box collision  */
	PaddleCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	PaddleCollision->SetupAttachment(RootComponent);
	PaddleCollision->SetBoxExtent(FVector(5.f, 160.f, 60.f));
	PaddleCollision->SetCollisionProfileName("BlockAll");

	/** set flip book  */
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> FlipbookTemplate(TEXT("/Game/Assets/Sprites/FB_Paddle"));
	if (FlipbookTemplate.Succeeded())
	{
		GetSprite()->SetFlipbook(FlipbookTemplate.Object);
	}
	GetSprite()->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	GetSprite()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCapsuleRadius(2.f);
}

void APaddle::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("Move", this, &APaddle::MoveUp);
}

void APaddle::MoveUp(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}
