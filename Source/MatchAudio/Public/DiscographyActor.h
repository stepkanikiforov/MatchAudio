// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DiscographyActor.generated.h"

class ATapeActor;

UCLASS()
class MATCHAUDIO_API ADiscographyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADiscographyActor();

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATapeActor> SpawnTapeActor;

	UFUNCTION(BlueprintCallable)
	void SpawnTapeBoxes(TArray<FString> Discography);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Border = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Gap = 35;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
