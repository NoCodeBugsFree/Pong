// Fill out your copyright notice in the Description page of Project Settings.

#include "PongGameMode.h"
#include "EngineUtils.h"
#include "Board.h"
#include "PongGameState.h"

void APongGameMode::BeginPlay()
{
	Super::BeginPlay();

	/** set game state reference  */
	PongGameState = GetGameState<APongGameState>();

	/** find and store board  */
	TArray<ABoard*> Boards;
	for (TActorIterator<ABoard> Iter(GetWorld()); Iter; ++Iter)
	{
		if ((*Iter))
		{
			Boards.Add(*Iter);
		}
	}

	/** if we found the board tell all players to set new view target */
	if (Boards.IsValidIndex(0))
	{
		Board = Boards[0];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Board is not found! place the board actor to the level"));
		return;
	}
}

void APongGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	/** if we have 2 players  */
	if (GetNumPlayers() == 2) 
	{
		/** camera lerp  */
		FTimerHandle SetCameraTimer;
		GetWorldTimerManager().SetTimer(SetCameraTimer, this, &APongGameMode::SetCameras, 1.f, false);
		
		/** start spawning  */
		FTimerDelegate StartSpawnDelegate;
		StartSpawnDelegate.BindLambda([&] 
		{
			/** start spawning  */
			if (Board)
			{
				Board->SetCanSpawn(true);
			}
		});
		
		FTimerHandle StartSpawnTimer;
		GetWorldTimerManager().SetTimer(StartSpawnTimer, StartSpawnDelegate, 3.f, false);
	}
}

void APongGameMode::SetCameras()
{
	if (PongGameState)
	{
		PongGameState->MulticastSetViewTarget(Board);
	}
}
