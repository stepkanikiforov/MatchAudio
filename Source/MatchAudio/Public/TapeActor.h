// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TapeActor.generated.h"

UCLASS()
class MATCHAUDIO_API ATapeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATapeActor();

	USceneComponent* Root = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* Body = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString SoundName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 SoundIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
