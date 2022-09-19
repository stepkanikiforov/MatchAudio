// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MatchAudioGameMode.generated.h"

UCLASS()
class MATCHAUDIO_API AMatchAudioGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMatchAudioGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentScore = 0;
};