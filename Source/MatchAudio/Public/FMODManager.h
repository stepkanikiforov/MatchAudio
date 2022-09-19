// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FMODSystem.h"
#include <vector>
#include "FMODManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateOverSound);

USTRUCT(BlueprintType)
struct FBandParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "20000", UIMin = "0", UIMax = "20000"))
	int32 StartFreq = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "20000", UIMin = "0", UIMax = "20000"))
	int32 StopFreq = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Coef = 0;
};

UCLASS(Blueprintable, BlueprintType)
class MATCHAUDIO_API UFMODManager : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UFMODManager();
	~UFMODManager();

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;

	UFUNCTION(BlueprintCallable, Category = Init)
	void LoadAndPrepareSoundFromMemory(FString Path);
	UFUNCTION(BlueprintCallable, Category = Actions)
	void Play();
	UFUNCTION(BlueprintCallable, Category = Actions)
	void PauseAnalyze(bool IsPause);
	UFUNCTION(BlueprintCallable, Category = Actions)
	void PausePlayer(bool IsPause);
	UFUNCTION(BlueprintCallable, Category = Actions)
	void Stop();

	UPROPERTY(BlueprintAssignable)
	FDelegateOverSound DelegateOverSound;

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBandParams> FreqArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumBars = 32;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Delay = 0.1f;

	// Output
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Output)
	TArray<float> SpectorStoreAnalyzer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Output)
	TArray<bool> BeatStoreAnalyzer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Output)
	TArray<float> SpectorStorePlayer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Output)
	TArray<bool> BeatStorePlayer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Output)
	float SPM = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Output)
	float PeakLevel = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Output)
	int32 SoundLength = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Output)
	TArray<FString> Discography;

private:
	void InitFMODSystem();
	void UpdateSystem();
	void FillDiscography();
	UFUNCTION()
	void PlaySound();

	FTimerHandle Timer;

	TUniquePtr<FMODSystem> _systemPlayer;
	TUniquePtr<FMODSystem> _systemAnalyzer;

	std::vector<float> BandCoef;

	bool isLoad = false;
};
