// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchTable.h"
#include "TileActor.h"
#include "MatchAudioGameMode.h"
#include "Kismet/GameplayStatics.h"

AMatchTable::AMatchTable()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	for (int32 i = 0; i < 16; ++i)
	{
		FTileData Tile;
		GridTiles.Add(Tile);
	}
}

void AMatchTable::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AMatchAudioGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AMatchTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMatchTable::PlaceTile(int32 NumTrack, int32 Type)
{
	int32 Index = 12 + NumTrack;
	if (TileTypes.Num() == 4)
	{
		if (GridTiles[Index].Type == -1)
		{
			FVector Location = FVector(4 * TileHeight, (TileWidth * NumTrack) - (1.5 * TileWidth), 0.0f);
			ATileActor* SpawnedActor = GetWorld()->SpawnActor<ATileActor>(TileActor, Location, this->GetActorRotation());
			if (SpawnedActor)
			{
				SpawnedActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				SpawnedActor->MatchTableInterface = this;
				SpawnedActor->SetMaterial(TileTypes[Type].Material);
				SpawnedActor->TileScore = TileTypes[Type].ScorePoints;
				SpawnedActor->TileHeight = TileHeight;
				SpawnedActor->NumTrack = NumTrack;
				SpawnedActor->Type = Type;
			}
		}
		else
		{
			DestroyTrackCollection(Index);
			DelegateTrackDestroy.Broadcast(NumTrack);
		}
	}
}

void AMatchTable::ClearTile(int32 Index)
{
	GridTiles[Index].Type = -1;
	GridTiles[Index].Tile = nullptr;
	GridTiles[Index].IsMatchCount = true;
}

int32 AMatchTable::GetTileType(int32 Index)
{
	return GridTiles[Index].Type;
}

void AMatchTable::SetTile(int32 Index, int32 Type, ATileActor* Tile)
{
	GridTiles[Index].Type = Type;
	GridTiles[Index].Tile = Tile;
	GridTiles[Index].IsMatchCount = true;
}

void AMatchTable::CalculateMatches(int32 Index, int32 Type)
{
	TArray<int32> MatchesInex;
	TArray<ATileActor*> MatchTile;
	FloodFill(MatchesInex, MatchTile, Index, Type);
	if (MatchesInex.Num() > 1)
	{
		bool IsFind = false;
		for (auto& m : TileMatches)
		{
			int32 count = 0;
			for (auto& t : MatchTile)
			{
				if (m.Contains(t))
				{
					count++;
				}
			}
			if (count == MatchTile.Num())
			{
				IsFind = true;
			}
		}

		if (!IsFind)
		{
			TileMatches.Add(MatchTile);
			for (auto& t : MatchesInex)
			{
				if (GridTiles[t].Tile && GridTiles[t].Type == Type)
				{
					GridTiles[t].Tile->ResetAnimationDisappear();
				}
			}
		}

		for (auto& t : MatchesInex)
		{
			if (GridTiles[t].Tile && GridTiles[t].Type == Type)
			{
				GridTiles[t].IsMatchCount = true;
			}
		}
	}
	if (MatchesInex.Num() == 1)
	{
		GridTiles[MatchesInex[0]].IsMatchCount = true;
	}
}

void AMatchTable::CalculateMatchesForScore(int32 Index, int32 Type, int32 TileScore, ATileActor* TileToShow)
{
	TArray<int32> MatchesInex;
	TArray<ATileActor*> MatchTile;
	FloodFill(MatchesInex, MatchTile, Index, Type);
	if (MatchesInex.Num() > 1)
	{
		if (GameMode)
		{
			GameMode->CurrentScore += MatchesInex.Num() * TileScore;
		}
		TileToShow->ShowCombo(MatchesInex.Num());
		DelegateComboScore.Broadcast(MatchesInex.Num(), Type);
		DelegateSingleScore.Broadcast(MatchesInex.Num() * TileScore, Type);
	}
	else if (MatchesInex.Num() == 1)
	{
		if (GameMode)
		{
			GameMode->CurrentScore += TileScore;
		}
		DelegateSingleScore.Broadcast(TileScore, Type);
	}

	TArray<TArray<ATileActor*>> Remove;
	for (auto& m : TileMatches)
	{
		for (auto& t : MatchTile)
		{
			if (m.Contains(t))
			{
				Remove.Add(m);
			}
		}
	}
	if (Remove.Num() > 0)
	{
		for (auto& r : Remove)
		{
			TileMatches.Remove(r);
		}
	}
}

void AMatchTable::DestroyTrackCollection(int32 Index)
{
	for (int32 i = 0; i <= Index / 4; ++i)
	{
		if (GridTiles[Index - i * 4].Tile)
		{
			TArray<TArray<ATileActor*>> Remove;
			for (auto& t : TileMatches)
			{
				if (t.Contains(GridTiles[Index - i * 4].Tile))
				{
					Remove.Add(t);
				}
			}
			if (Remove.Num() > 0)
			{
				for (auto& r : Remove)
				{
					TileMatches.Remove(r);
				}
			}

			GridTiles[Index - i * 4].Tile->Destroy();
			GridTiles[Index - i * 4].Tile = nullptr;
			GridTiles[Index - i * 4].Type = -1;
		}
	}
}

void AMatchTable::FloodFill(TArray<int32>& MatchArray, TArray<ATileActor*>& MatchTiles, int32 Index, int32 Type)
{
	if (Index >= GridTiles.Num() || Index < 0 || GridTiles[Index].Type != Type || !GridTiles[Index].IsMatchCount)
	{
		return;
	}
	else
	{
		MatchArray.Add(Index);
		MatchTiles.Add(GridTiles[Index].Tile);
		GridTiles[Index].IsMatchCount = false;
		FloodFill(MatchArray, MatchTiles, Index + 4, Type);
		FloodFill(MatchArray, MatchTiles, Index - 4, Type);
		if ((Index + 1) % 4 != 0)
		{
			FloodFill(MatchArray, MatchTiles, Index + 1, Type);
		}
		if (Index % 4 != 0)
		{
			FloodFill(MatchArray, MatchTiles, Index - 1, Type);
		}
	}
}
