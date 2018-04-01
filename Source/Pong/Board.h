// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "Board.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API ABoard : public APaperSpriteActor
{
	GENERATED_BODY()
	
	/* up box */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* UpBox;
	
	/* bottom  box */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BottomBox;
	
	/* player01 goal box */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Player01Box;
	
	/* player02 goal box */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Player02Box;
	
	/* camera component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* LevelCamera;
	
	/* arrow component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* SpawnPoint;
	
public:

	/** called to start spawning new balls */
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "AAA")
	void SetCanSpawn(bool bNewCanSpawn);

	/** called to set AI Paddle reference */
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "AAA")
	void SetAI_Paddle(class AAI_Paddle* NewAI_Paddle) { AI_Paddle = NewAI_Paddle; }

	/** called to nullify ball reference in AI Paddle */
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "AAA")
	void NillifyBallRef();

protected:

	ABoard();

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

private:

	/** AI_Paddle reference to update on-board ball  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class AAI_Paddle* AI_Paddle;

	/** shows whether we can spawn new ball or not  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	uint32 bCanSpawn : 1;
	
	/** game state reference  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class APongGameState* PongGameState;

	/** ball class template  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABall> BallTemplate;

	/** spawn timer  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FTimerHandle SpawnTimer;

	/** timer interval to spawn new ball after previous ball was destroyed  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float SpawnDelay = 3.f;

	// -----------------------------------------------------------------------------------

	/** called to spawn a new ball */
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category = "AAA")
	void SpawnBall();

	/** called when player 01 goal box overlap begin  */
	UFUNCTION()
	void OnPlayer01BoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** called when player 02 goal box overlap begin  */
	UFUNCTION()
	void OnPlayer02BoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
