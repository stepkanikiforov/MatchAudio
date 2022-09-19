// Fill out your copyright notice in the Description page of Project Settings.

#include "BackgroundSky.h"
#include "Materials/MaterialInstanceDynamic.h"

ABackgroundSky::ABackgroundSky()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Background = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Background"));
	Background->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Background->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Background->CastShadow = false;

}

void ABackgroundSky::BeginPlay()
{
	Super::BeginPlay();
	if (Material)
	{
		DynMaterial = UMaterialInstanceDynamic::Create(Material, this);
		if (DynMaterial)
		{
			Background->SetMaterial(0, DynMaterial);
		}
	}
}

void ABackgroundSky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABackgroundSky::ShowBeat()
{
	if (bPermitionToShowBeat)
	{
		if (DynMaterial)
		{
			float GameTime = GetWorld()->GetTimeSeconds();
			DynMaterial->SetScalarParameterValue("GameTime", GameTime);
		}
		bPermitionToShowBeat = false;
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &ABackgroundSky::PermitionToShowBeat, 0.4, false);
	}
}

void ABackgroundSky::PermitionToShowBeat()
{
	bPermitionToShowBeat = true;
	GetWorld()->GetTimerManager().ClearTimer(Timer);
}
