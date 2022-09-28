// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TapePawn.generated.h"

class UCameraComponent;
class UBoxComponent;
class AMatchTable;

UCLASS()
class MATCHAUDIO_API ATapePawn : public APawn
{
	GENERATED_BODY()

public:
	ATapePawn();

	USceneComponent* Root = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCameraComponent* Camera = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* MoveableComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Body = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* Collision = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SpeedMovement = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float LocalBorder = 150;

	UFUNCTION(BlueprintCallable)
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMatchTable* MatchTable = nullptr;

private:
	void TouchPressed();
	bool IsTouch = false;
	APlayerController* PlayerController = nullptr;
	FVector2D StartLocation = FVector2D(0, 0);
	FVector2D FinishtLocation = FVector2D(0, 0);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
