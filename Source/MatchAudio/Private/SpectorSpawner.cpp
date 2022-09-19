// Fill out your copyright notice in the Description page of Project Settings.


#include "SpectorSpawner.h"
#include "SpectorBarActor.h"

ASpectorSpawner::ASpectorSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASpectorSpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnActors();
}

void ASpectorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpectorSpawner::SpawnActors()
{
	SpectorActors.Reserve(NumBars);
	for (int32 i = 0; i < NumBars; ++i)
	{
		FVector Location = FVector(i * 100, 0.0f, 0.0f);
		ASpectorBarActor* SpawnedActor = GetWorld()->SpawnActor<ASpectorBarActor>(SpawnSpectorActor, Location, this->GetActorRotation());
		if (SpawnedActor)
		{
			SpawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			SpectorActors.Add(SpawnedActor);
			SpawnedActor->SetInitParameters(SpeedGrowth, SpeedFade, Material);
		}
	}
}
