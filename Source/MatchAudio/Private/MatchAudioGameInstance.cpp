// Fill out your copyright notice in the Description page of Project Settings.

#include "MatchAudioGameInstance.h"
#include "FMODManager.h"
#include "Kismet/GameplayStatics.h"
#include "MatchAudioSaveGame.h"

void UMatchAudioGameInstance::Init()
{
	Super::Init();
	SaveGameSlotName = TEXT("Save");
	LoadSaveGame();
}

void UMatchAudioGameInstance::LoadSaveGame()
{
	UMatchAudioSaveGame* SaveGameObjectInstance = Cast<UMatchAudioSaveGame>(UGameplayStatics::CreateSaveGameObject(UMatchAudioSaveGame::StaticClass()));
	SaveGameObjectInstance = Cast<UMatchAudioSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0));
	if (SaveGameObjectInstance == nullptr)
	{
		UMatchAudioSaveGame* FirstSaveGameObjectInstance = Cast<UMatchAudioSaveGame>(UGameplayStatics::CreateSaveGameObject(UMatchAudioSaveGame::StaticClass()));
		UGameplayStatics::SaveGameToSlot(FirstSaveGameObjectInstance, SaveGameSlotName, 0);
		TotalScore = FirstSaveGameObjectInstance->TotalScore;
	}
	else
	{
		TotalScore = SaveGameObjectInstance->TotalScore;
	}
}

void UMatchAudioGameInstance::SaveGame()
{
	UMatchAudioSaveGame* SaveGameObjectInstance = Cast<UMatchAudioSaveGame>(UGameplayStatics::CreateSaveGameObject(UMatchAudioSaveGame::StaticClass()));
	SaveGameObjectInstance->TotalScore = TotalScore;
	UGameplayStatics::SaveGameToSlot(SaveGameObjectInstance, SaveGameSlotName, 0);
}
