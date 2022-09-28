// Fill out your copyright notice in the Description page of Project Settings.


#include "DiscographyActor.h"
#include "TapeActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainMenuSpectatorPawn.h"

ADiscographyActor::ADiscographyActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADiscographyActor::BeginPlay()
{
	Super::BeginPlay();

}

void ADiscographyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void ADiscographyActor::SpawnTapeBoxes(TArray<FString> Discography)
{
	for (int32 i = 0; i < Discography.Num(); ++i)
	{
		SpawnBorder = i * Gap;
		ATapeActor* SpawnedTapeActor = GetWorld()->SpawnActor<ATapeActor>(SpawnTapeActor, 
																		  FVector(0, UKismetMathLibrary::RandomFloatInRange(-HorizontalOffset, HorizontalOffset), SpawnBorder * (-1)),
																		  FRotator(0, 0, UKismetMathLibrary::RandomFloatInRange(-AngleOffset, AngleOffset)));
		if (SpawnedTapeActor)
		{
			SpawnedTapeActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			SpawnedTapeActor->SoundName = Discography[i];
			SpawnedTapeActor->SoundIndex = i;
		}
	}

	AMainMenuSpectatorPawn* Pawn = Cast<AMainMenuSpectatorPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(Pawn)
	{
		Pawn->SetBorder(SpawnBorder);
	}
}
