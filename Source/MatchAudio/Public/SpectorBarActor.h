// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpectorBarActor.generated.h"

UCLASS()
class MATCHAUDIO_API ASpectorBarActor : public AActor
{
	GENERATED_BODY()
	
public:
	ASpectorBarActor();
	virtual void Tick(float DeltaTime) override;

	USceneComponent* Root = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Body = nullptr;

	UFUNCTION(BlueprintCallable)
	void SetTargetScaleZ(float Scale);
	void SetInitParameters(float Growth, float Fade, UMaterialInterface* SetMaterial);


protected:
	virtual void BeginPlay() override;

private:
	float TargetScaleZ = 0;
	float SpeedGrowth = 100;
	float SpeedFade = 100;

	UPROPERTY()
	UMaterialInstanceDynamic* DynMaterialGradient = nullptr;

};
