// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Paddle.h"
#include "AI_Paddle.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API AAI_Paddle : public APaddle
{
	GENERATED_BODY()

public:

	/** called to set new ball reference  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SetBall(class ABall* BallToSet) { Ball = BallToSet; }
	
protected:
	
	/** store initial location  */
	virtual void BeginPlay() override;

	/** AI logic  */
	virtual void Tick(float DeltaSeconds) override;

private:

	/** on board ball reference  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class ABall* Ball;

	/** AI initial location  */
	UPROPERTY()
	FVector InitialLocation;
};
