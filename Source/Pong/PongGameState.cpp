// Fill out your copyright notice in the Description page of Project Settings.

#include "PongGameState.h"
#include "Board.h"
#include "Net/UnrealNetwork.h"

void APongGameState::MulticastSetViewTarget_Implementation(class ABoard* Board)
{
	/** for each pawn in the world  */
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
	{
		if (APawn* Pawn = Cast<APawn>(*It))
		{
			/** valid and locally controlled pawn  */
			if (Pawn->IsLocallyControlled())
			{
				if (APlayerController* PC = Cast<APlayerController>(Pawn->GetController()))
				{
					/** swap cameras  */
					if (Board)
					{
						PC->SetViewTargetWithBlend(Board, 1.f, EViewTargetBlendFunction::VTBlend_Cubic);
					}
				}
			}
		}
	}
}

/** Returns properties that are replicated for the lifetime of the actor channel */
void APongGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APongGameState, Player01Score);
	DOREPLIFETIME(APongGameState, Player02Score);
}