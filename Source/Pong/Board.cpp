// Fill out your copyright notice in the Description page of Project Settings.

#include "Board.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "PongGameState.h"
#include "Ball.h"
#include "AI_Paddle.h"

ABoard::ABoard()
{
	/**  paper sprite component */
	SetRootComponent(GetRenderComponent());

	/** set sprite asset */
	static ConstructorHelpers::FObjectFinder<UPaperSprite> PaperSpriteTemplate(TEXT("/Game/Assets/Sprites/BG_Sprite2"));
	if (PaperSpriteTemplate.Succeeded())
	{
		GetRenderComponent()->SetSprite(PaperSpriteTemplate.Object);
	}

	/* up box */
	UpBox = CreateDefaultSubobject<UBoxComponent>(TEXT("UpBox"));
	UpBox->SetupAttachment(RootComponent);
	UpBox->SetBoxExtent(FVector(1200.f, 500.f, 100.f));

	UpBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	UpBox->SetCollisionProfileName("BlockAll");
	UpBox->SetRelativeLocation(FVector(0.f, 0.f, 670.f));

	/* bottom  box */
	BottomBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BottomBox"));
	BottomBox->SetupAttachment(RootComponent);
	BottomBox->SetBoxExtent(FVector(1200.f, 500.f, 100.f));

	BottomBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BottomBox->SetCollisionProfileName("BlockAll");
	BottomBox->SetRelativeLocation(FVector(0.f, 0.f, -670.f));

	/* player 01 goal box */
	Player01Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Player01Box"));
	Player01Box->SetupAttachment(RootComponent);
	Player01Box->SetBoxExtent(FVector(100.f, 500.f, 800.f));
	Player01Box->SetRelativeLocation(FVector(-1090.f, 0.f, 0.f));

	Player01Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Player01Box->SetCollisionProfileName("OverlapAll");

	/* player 02 goal box */
	Player02Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Player02Box"));
	Player02Box->SetupAttachment(RootComponent);
	Player02Box->SetBoxExtent(FVector(100.f, 500.f, 800.f));
	Player02Box->SetRelativeLocation(FVector(1090.f, 0.f, 0.f));

	Player02Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Player02Box->SetCollisionProfileName("OverlapAll");

	/* camera component  */
	LevelCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	LevelCamera->SetupAttachment(RootComponent);
	LevelCamera->SetRelativeLocation(FVector(0.f, 1300.f, 0.f));
	LevelCamera->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	/* arrow component  */
	SpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootComponent);
	SpawnPoint->ArrowSize = 5.f;
	SpawnPoint->SetRelativeRotation(FRotator(50.f, 0.f, 0.f));
	SpawnPoint->SetRelativeLocation(FVector(0.f, 100.f, 0.f));

	/**  set ball class to spawn */
	static ConstructorHelpers::FClassFinder<ABall> BallBPClass(TEXT("/Game/BP/BP_Ball"));
	if (BallBPClass.Class)
	{
		BallTemplate = BallBPClass.Class;
	}

	/** class defaults  */
	bCanSpawn = false;
}

void ABoard::BeginPlay()
{
	Super::BeginPlay();

	/** set GS reference  */
	if(APongGameState* TestGS = Cast<APongGameState>(GetWorld()->GetGameState()))
	{
		PongGameState = TestGS;
	}

	/** start spawn timer  */
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ABoard::SpawnBall, SpawnDelay, true);
}

void ABoard::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (Role == ROLE_Authority)
	{
		Player01Box->OnComponentBeginOverlap.AddDynamic(this, &ABoard::OnPlayer01BoxOverlapBegin);
		Player02Box->OnComponentBeginOverlap.AddDynamic(this, &ABoard::OnPlayer02BoxOverlapBegin);
	}
}

void ABoard::SetCanSpawn(bool bNewCanSpawn)
{
	if (Role == ROLE_Authority)
	{
		bCanSpawn = bNewCanSpawn;
	}
}

void ABoard::NillifyBallRef()
{
	if (AI_Paddle)
	{
		AI_Paddle->SetBall(nullptr);
	}
}

void ABoard::SpawnBall()
{
	if (Role == ROLE_Authority && bCanSpawn)
	{
		if (BallTemplate)
		{
			UWorld* const World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				
				FTransform SpawnTransform = SpawnPoint->GetComponentTransform();
				
				ABall* SpawnedBall = World->SpawnActor<ABall>(BallTemplate, SpawnTransform, SpawnParams);
				if (SpawnedBall)
				{
					/** set ball reference in AI Paddle  */
					if (AI_Paddle)
					{
						AI_Paddle->SetBall(SpawnedBall);
					}

					bCanSpawn = false;
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("BallTemplate == NULL"));
		}
	}
}

void ABoard::OnPlayer01BoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Role == ROLE_Authority)
	{
		// Other Actor is the actor that triggered the event. Check that is not ourself. 
		if ((OtherActor != nullptr) && (OtherActor != this) && (!OtherActor->IsPendingKill()) && OtherActor->IsA(ABall::StaticClass()))
		{
			/** add player 1 score  */
			if (PongGameState)
			{
				PongGameState->Player02Score++;
			}
			OtherActor->Destroy();
		}
	}
}

void ABoard::OnPlayer02BoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Role == ROLE_Authority)
	{
		// Other Actor is the actor that triggered the event. Check that is not ourself. 
		if ((OtherActor != nullptr) && (OtherActor != this) && (!OtherActor->IsPendingKill()) && OtherActor->IsA(ABall::StaticClass()))
		{
			/** add player 1 score  */
			if (PongGameState)
			{
				PongGameState->Player01Score++;
			}
			OtherActor->Destroy();
		}
	}
}
