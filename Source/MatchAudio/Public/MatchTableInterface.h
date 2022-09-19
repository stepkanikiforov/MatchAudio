// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MatchTableInterface.generated.h"

class ATileActor;

UINTERFACE(MinimalAPI)
class UMatchTableInterface : public UInterface
{
	GENERATED_BODY()
};

class MATCHAUDIO_API IMatchTableInterface
{
	GENERATED_BODY()

public:
	virtual void CalculateMatches(int32 Index, int32 Type) = 0;
	virtual void CalculateMatchesForScore(int32 Index, int32 Type, int32 TileScore, ATileActor* TileToShow) = 0;
	virtual void DestroyTrackCollection(int32 Index) = 0;
	virtual void ClearTile(int32 Index) = 0;
	virtual int32 GetTileType(int32 Index) = 0;
	virtual void SetTile(int32 Index, int32 Type, ATileActor* Tile) = 0;
};
