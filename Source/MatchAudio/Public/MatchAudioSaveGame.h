// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MatchAudioSaveGame.generated.h"

UCLASS()
class MATCHAUDIO_API UMatchAudioSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UMatchAudioSaveGame();

	UPROPERTY(VisibleAnywhere)
	FString SaveSlotName;
	UPROPERTY(VisibleAnywhere)
	uint32 UserIndex;
	UPROPERTY(VisibleAnywhere)
	int32 TotalScore;
};
