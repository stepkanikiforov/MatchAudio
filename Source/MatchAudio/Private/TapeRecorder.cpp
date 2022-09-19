// Fill out your copyright notice in the Description page of Project Settings.


#include "TapeRecorder.h"
#include "BeatActor.h"
#include "Kismet/KismetMathLibrary.h"

ATapeRecorder::ATapeRecorder()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Body->CastShadow = false;

	BeatsSpawner = CreateDefaultSubobject<USceneComponent>(TEXT("BeatsSpawner"));
	BeatsSpawner->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
}

void ATapeRecorder::BeginPlay()
{
	Super::BeginPlay();
}

void ATapeRecorder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATapeRecorder::SpawnBeatByNum(uint8 NumBeat)
{
	if (NumBeat < SpawnBeats.Num() && SpawnBeatActor && SpawnBeats[NumBeat].bPermitionToSpawn)
	{
		int32 TrackIndex = GetTrackNumber();
		if (TrackIndex >= 0)
		{
			Tracks[TrackIndex].bPermitionToOccupy = false;
			Tracks[TrackIndex].Delegate.BindUFunction(this, "EnablePermitionToOccupy", TrackIndex);
			GetWorld()->GetTimerManager().SetTimer(Tracks[TrackIndex].Timer, Tracks[TrackIndex].Delegate, 0.4, false);
			SpawnBeats[NumBeat].bPermitionToSpawn = false;
			SpawnBeats[NumBeat].Delegate.BindUFunction(this, "EnablePermitionToSpawn", NumBeat);
			GetWorld()->GetTimerManager().SetTimer(SpawnBeats[NumBeat].Timer, SpawnBeats[NumBeat].Delegate, 0.4, false);
			FVector Location = BeatsSpawner->GetComponentLocation() + FVector(0.0f, Tracks[TrackIndex].DeltaLoacation, 0.0f);
			ABeatActor* SpawnedBeat = GetWorld()->SpawnActor<ABeatActor>(SpawnBeatActor, Location, BeatsSpawner->GetComponentRotation());
			if (SpawnedBeat)
			{
				SpawnedBeat->Body->SetMaterial(0, SpawnBeats[NumBeat].Material);
				SpawnedBeat->Type = NumBeat;
				SpawnedBeat->NumTrack = TrackIndex;
			}
		}
	}
}

void ATapeRecorder::EnablePermitionToSpawn(uint8 NumBeat)
{
	SpawnBeats[NumBeat].bPermitionToSpawn = true;
	GetWorld()->GetTimerManager().ClearTimer(SpawnBeats[NumBeat].Timer);
}

void ATapeRecorder::EnablePermitionToOccupy(int32 NumTrack)
{
	Tracks[NumTrack].bPermitionToOccupy = true;
	GetWorld()->GetTimerManager().ClearTimer(Tracks[NumTrack].Timer);
}

int32 ATapeRecorder::GetTrackNumber()
{
	TArray<int32> AvailiableTracks;
	for (int32 i = 0; i < Tracks.Num(); ++i)
	{
		if (Tracks[i].bPermitionToOccupy)
		{
			AvailiableTracks.Add(i);
		}
	}
	if (AvailiableTracks.Num() > 0)
	{
		int32 RandIndex = UKismetMathLibrary::RandomInteger(AvailiableTracks.Num());
		return AvailiableTracks[RandIndex];
	}

	return -1;
}
