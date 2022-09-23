// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeatTypeStruct.h"
#include "MatchTableInterface.h"
#include "MatchTable.generated.h"

class ATileActor;
class AMatchAudioGameMode;
class AComboActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegateComboScore, int32, Combo, int32, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegateSingleScore, int32, Score, int32, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateTrackDestroy, int32, Track);

USTRUCT()
struct FTileData
{
	GENERATED_BODY()

public:
	int32 Type = -1;
	UPROPERTY()
	ATileActor* Tile = nullptr;
	bool IsMatchCount = true;
};

UCLASS()
class MATCHAUDIO_API AMatchTable : public AActor, public IMatchTableInterface
{
	GENERATED_BODY()

public:
	AMatchTable();
	USceneComponent* Root = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATileActor> TileActor;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AComboActor> ComboActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TileWidth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TileHeight = 100;
	UPROPERTY(EditAnywhere)
	TArray<FBeatTypeStruct> TileTypes;

	TArray<FTileData> GridTiles;

	UPROPERTY(BlueprintAssignable)
	FDelegateComboScore DelegateComboScore;
	UPROPERTY(BlueprintAssignable)
	FDelegateSingleScore DelegateSingleScore;
	UPROPERTY(BlueprintAssignable)
	FDelegateTrackDestroy DelegateTrackDestroy;


	void PlaceTile(int32 NumTrack, int32 Type);

	void CalculateMatches(int32 Index, int32 Type) override;
	void CalculateMatchesForScore(int32 Index, int32 Type, int32 TileScore, ATileActor* TileToShow) override;
	void DestroyTrackCollection(int32 Index) override;
	void ClearTile(int32 Index) override;
	int32 GetTileType(int32 Index) override;
	void SetTile(int32 Index, int32 Type, ATileActor* Tile) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	AMatchAudioGameMode* GameMode = nullptr;
	void FloodFill(TArray<int32>& MatchArray, TArray<ATileActor*>& MatchTiles, int32 Index, int32 Type);
	TArray<TArray<ATileActor*>> TileMatches;
};
