// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Paddle.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API APaddle : public APaperCharacter
{
	GENERATED_BODY()
	
	/* box collision  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* PaddleCollision;
	
protected:

	APaddle();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	/** called to move player up/down  */
	void MoveUp(float Value);
	
};
