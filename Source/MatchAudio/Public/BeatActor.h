// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeatActor.generated.h"

class UBoxComponent;

UCLASS()
class MATCHAUDIO_API ABeatActor : public AActor
{
	GENERATED_BODY()

public:
	ABeatActor();

	USceneComponent* Root = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Body = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* Collision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters")
	int32 SpeedMovement = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Type = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumTrack = -1;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
