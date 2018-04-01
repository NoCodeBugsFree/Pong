// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PongGameState.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API APongGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	/** broadcasts set   */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetViewTarget(class ABoard* Board);
	void MulticastSetViewTarget_Implementation(class ABoard* Board);
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Config")
	int32 Player01Score = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Config")
	int32 Player02Score = 0;

protected:



private:
	
	
};
