// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BackgroundSky.generated.h"

UCLASS()
class MATCHAUDIO_API ABackgroundSky : public AActor
{
	GENERATED_BODY()
	
public:	
	ABackgroundSky();

	USceneComponent* Root = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Background = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material = nullptr;
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ShowBeat();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UMaterialInstanceDynamic* DynMaterial = nullptr;
	void PermitionToShowBeat();
	FTimerHandle Timer;
	bool bPermitionToShowBeat = true;
};
