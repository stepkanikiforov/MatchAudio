// Fill out your copyright notice in the Description page of Project Settings.


#include "TileActor.h"
#include "MatchAudioGameMode.h"
#include "Components/WidgetComponent.h"

ATileActor::ATileActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ComboWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Combo"));
	ComboWidget->AttachToComponent(Body, FAttachmentTransformRules::KeepRelativeTransform);
	ComboWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATileActor::BeginPlay()
{
	Super::BeginPlay();
}

void ATileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Index == -1)
	{
		int32 IndexToSet = -1;
		int32 DeltaLocation = 0;
		for (int32 i = 0; i < 4; ++i)
		{
			if (MatchTableInterface && NumTrack != -1 && MatchTableInterface->GetTileType(12 + NumTrack - 4 * i) == -1)
			{
				IndexToSet = 12 + NumTrack - 4 * i;
				DeltaLocation = TileHeight * i;
			}
		}
		if (IndexToSet != -1)
		{
			Index = IndexToSet;
			TargetLocation = GetActorLocation();
			TargetLocation.X = TargetLocation.X - DeltaLocation;
			MatchTableInterface->SetTile(Index, Type, this);
			IsSwapAnimation = true;
		}
	}

	if (MatchTableInterface && !IsSwapAnimation && Index != -1)
	{
		int32 EmptyIndex = Index;
		int32 DeltaLocation = 0;
		for (int32 i = 1; i <= Index / 4; ++i)
		{
			if (MatchTableInterface->GetTileType(Index - 4 * i) == -1)
			{
				EmptyIndex = Index - 4 * i;
				DeltaLocation = TileHeight * i;
			}
			else
			{
				break;
			}
		}

		if (EmptyIndex != Index)
		{
			TargetLocation = GetActorLocation();
			TargetLocation.X = TargetLocation.X - DeltaLocation;

			MatchTableInterface->ClearTile(Index);
			Index = EmptyIndex;
			MatchTableInterface->SetTile(Index, Type, this);
			IsSwapAnimation = true;
		}
	}

	if (IsSwapAnimation)
	{
		SwapAnimation(DeltaTime);
	}
	if (!IsDisappear)
	{
		AnimateDisappear(DeltaTime);
	}
}

void ATileActor::SwapAnimation(float DeltaTime)
{
	if ((GetActorLocation().X - TargetLocation.X) > 0)
	{
		FVector Location = GetActorLocation();
		Location.X = Location.X - DeltaTime * SwapAnimationSpeed;
		SetActorLocation(Location);
	}
	else
	{
		IsSwapAnimation = false;
		SetActorLocation(TargetLocation);
		if (MatchTableInterface)
		{
			MatchTableInterface->CalculateMatches(Index, Type);
		}
	}
}

void ATileActor::SetMaterial(UMaterialInstance* Material)
{
	Body->SetMaterial(0, Material);
}

void ATileActor::AnimateDisappear(float DeltaTime)
{
	if (Body && Index != -1)
	{
		FVector Location = Body->GetRelativeLocation();
		if (HiddenHeight > -20)
		{
			HiddenHeight = HiddenHeight - DeltaTime * DisappearAnimationSpeed;
		}
		else
		{
			HiddenHeight = -20;
			if (MatchTableInterface)
			{
				MatchTableInterface->CalculateMatchesForScore(Index, Type, TileScore, this);
				MatchTableInterface->ClearTile(Index);
				IsDisappear = true;
				Destroy();
			}
		}
		Location.Z = HiddenHeight;
		Body->SetRelativeLocation(Location);
	}
}

void ATileActor::DeferredDestroy()
{
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ATileActor::DestroyTile, 2.0, false);
}

void ATileActor::DestroyTile()
{
	GetWorld()->GetTimerManager().ClearTimer(Timer);
	Destroy();
}

void ATileActor::ResetAnimationDisappear()
{
	if (Body)
	{
		HiddenHeight = 0;
		FVector Location = Body->GetRelativeLocation();
		Location.Z = HiddenHeight;
		Body->SetRelativeLocation(Location);
	}
}

void ATileActor::ShowCombo(int32 Combo)
{
	DelegateCombo.Broadcast(Combo, Type);
}
