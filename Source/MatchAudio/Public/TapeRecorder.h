// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TapeRecorder.generated.h"

class ABeatActor;

USTRUCT()
struct FBeatSpawnData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UMaterialInstance* Material = nullptr;
	bool bPermitionToSpawn = true;
	FTimerHandle Timer;
	FTimerDelegate Delegate;
};

USTRUCT()
struct FTrackSpawnData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float DeltaLoacation = 0;
	bool bPermitionToOccupy = true;
	FTimerHandle Timer;
	FTimerDelegate Delegate;
};

UCLASS()
class MATCHAUDIO_API ATapeRecorder : public AActor
{
	GENERATED_BODY()

public:
	ATapeRecorder();

	virtual void Tick(float DeltaTime) override;

	USceneComponent* Root = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Body = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USceneComponent* BeatsSpawner = nullptr;

	UPROPERTY(EditAnywhere, Category = "SpawnBeats")
	TArray<FBeatSpawnData> SpawnBeats;
	UPROPERTY(EditAnywhere, Category = "SpawnBeats")
	TArray<FTrackSpawnData> Tracks;

	UFUNCTION(BlueprintCallable)
	void SpawnBeatByNum(uint8 NumBeat);

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABeatActor> SpawnBeatActor;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void EnablePermitionToSpawn(uint8 NumBeat);
	UFUNCTION()
	void EnablePermitionToOccupy(int32 NumTrack);

	int32 GetTrackNumber();
};
