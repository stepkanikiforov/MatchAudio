// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "MatchAudioGameInstance.generated.h"

class UFMODManager;
class UMatchAudioSaveGame;

UCLASS()
class MATCHAUDIO_API UMatchAudioGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalScore = 0;
	UPROPERTY(BlueprintReadOnly)
	FString SaveGameSlotName;

	void LoadSaveGame();
	UFUNCTION(BlueprintCallable)
	void SaveGame();
};
