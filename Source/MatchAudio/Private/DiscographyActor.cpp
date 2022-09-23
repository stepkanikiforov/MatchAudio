// Fill out your copyright notice in the Description page of Project Settings.


#include "DiscographyActor.h"
#include "TapeActor.h"

// Sets default values
ADiscographyActor::ADiscographyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADiscographyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADiscographyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADiscographyActor::SpawnTapeBoxes(TArray<FString> Discography)
{
	for (int32 i = 0; i < Discography.Num(); ++i)
	{
		Border = i * (-32);
		ATapeActor* SpawnedTapeActor = GetWorld()->SpawnActor<ATapeActor>(SpawnTapeActor, FVector(0, 0, Border), FRotator::ZeroRotator);
		if (SpawnedTapeActor)
		{
			SpawnedTapeActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			SpawnedTapeActor->SoundName = Discography[i];
			SpawnedTapeActor->SoundIndex = i;
		}
	}
}
