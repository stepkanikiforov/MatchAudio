// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpectorSpawner.generated.h"

class ASpectorBarActor;

UCLASS()
class MATCHAUDIO_API ASpectorSpawner : public AActor
{
	GENERATED_BODY()

public:
	ASpectorSpawner();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASpectorBarActor> SpawnSpectorActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ASpectorBarActor*> SpectorActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	int32 NumBars = 32;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float SpeedGrowth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float SpeedFade = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	UMaterialInterface* Material = nullptr;

protected:
	virtual void BeginPlay() override;

private:
	void SpawnActors();
};
