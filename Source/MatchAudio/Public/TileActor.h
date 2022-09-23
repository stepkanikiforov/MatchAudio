// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MatchTableInterface.h"
#include "TileActor.generated.h"

class AMatchAudioGameMode;
class UWidgetComponent;

UCLASS()
class MATCHAUDIO_API ATileActor : public AActor
{
	GENERATED_BODY()

public:
	ATileActor();

	virtual void Tick(float DeltaTime) override;

	USceneComponent* Root = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Body = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* ComboWidget = nullptr;

	int32 Index = -1;
	int32 Type = -1;
	int32 NumTrack = -1;
	int32 TileScore = 0;
	float TileHeight = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SwapAnimationSpeed = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DisappearAnimationSpeed = 1;

	void SetMaterial(UMaterialInstance* Material);
	void ResetAnimationDisappear();

	IMatchTableInterface* MatchTableInterface = nullptr;

	void DeferredDestroy();
protected:
	virtual void BeginPlay() override;

private:
	bool IsSwapAnimation = false;
	FVector TargetLocation;

	void AnimateDisappear(float DeltaTime);
	void SwapAnimation(float DeltaTime);

	float HiddenHeight = 0;
	bool IsDisappear = false;
	FTimerHandle Timer;
	void DestroyTile();
};
