// Fill out your copyright notice in the Description page of Project Settings.

#include "SpectorBarActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

ASpectorBarActor::ASpectorBarActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Body->CastShadow = false;
}

void ASpectorBarActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASpectorBarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetDelta = Body->GetRelativeScale3D().Z - TargetScaleZ;

	if (TargetDelta > 0)
	{
		float ScaleToSet = Body->GetRelativeScale3D().Z - DeltaTime * UKismetMathLibrary::Abs(TargetDelta) * SpeedFade;
		if (ScaleToSet > TargetScaleZ)
		{
			Body->SetRelativeScale3D(FVector(1, 1, ScaleToSet));
		}
		else
		{
			Body->SetRelativeScale3D(FVector(1, 1, TargetScaleZ));
		}
		if (DynMaterialGradient)
		{
			DynMaterialGradient->SetScalarParameterValue("Hight", 500 / ScaleToSet);
		}
	}
	if (TargetDelta < 0)
	{
		float ScaleToSet = Body->GetRelativeScale3D().Z + DeltaTime * UKismetMathLibrary::Abs(TargetDelta) * SpeedGrowth;
		if (ScaleToSet < TargetScaleZ)
		{
			Body->SetRelativeScale3D(FVector(1, 1, ScaleToSet));
		}
		else
		{
			Body->SetRelativeScale3D(FVector(1, 1, TargetScaleZ));
		}
		if (DynMaterialGradient)
		{
			DynMaterialGradient->SetScalarParameterValue("Hight", 500 / ScaleToSet);
		}
	}
}

void ASpectorBarActor::SetTargetScaleZ(float Scale)
{
	TargetScaleZ = Scale;
}

void ASpectorBarActor::SetInitParameters(float Growth, float Fade, UMaterialInterface* SetMaterial)
{
	SpeedGrowth = Growth;
	SpeedFade = Fade;
	if (Body && SetMaterial)
	{
		DynMaterialGradient = UMaterialInstanceDynamic::Create(SetMaterial, this);
		if (DynMaterialGradient)
		{
			Body->SetMaterial(0, DynMaterialGradient);
		}
	}
}
