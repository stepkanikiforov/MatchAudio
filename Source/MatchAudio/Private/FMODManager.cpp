// Fill out your copyright notice in the Description page of Project Settings.


#include "FMODManager.h"
#include "GenericPlatform/GenericPlatformFile.h"

UFMODManager::UFMODManager()
{
	InitFMODSystem();
}

UFMODManager::~UFMODManager()
{
	if (_systemPlayer)
	{
		_systemPlayer->~FMODSystem();
	}
	if (_systemAnalyzer)
	{
		_systemAnalyzer->~FMODSystem();
	}
}

void UFMODManager::InitFMODSystem()
{
	_systemPlayer = MakeUnique<FMODSystem>();
	if (_systemPlayer)
	{
		_systemPlayer->InitializeSystem(true);
	}

	_systemAnalyzer = MakeUnique<FMODSystem>();
	if (_systemAnalyzer)
	{
		_systemAnalyzer->InitializeSystem(false);
	}
	FillDiscography();
}

void UFMODManager::FillDiscography()
{
	Discography.Empty();

#if PLATFORM_ANDROID
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	extern FString GFilePathBase;
	FString androidFullPath = GFilePathBase + FString("/Download/");
	PlatformFile.FindFilesRecursively(Discography, *androidFullPath, TEXT(".mp3"));
#endif
}

void UFMODManager::LoadAndPrepareSoundFromMemory(FString Path)
{
	if (_systemAnalyzer && _systemPlayer)
	{
		uint8* data;
		unsigned int dataLength = 0;

		TArray <uint8> rawFile;

		FFileHelper::LoadFileToArray(rawFile, *Path);

		data = rawFile.GetData();
		dataLength = rawFile.Num() * sizeof(uint8);

		_systemAnalyzer->LoadSoundFromMemory(reinterpret_cast<char*>(data), dataLength);
		_systemAnalyzer->Play();
		_systemAnalyzer->Pause(true);
		SoundLength = _systemAnalyzer->GetSoundLendth();

		BandCoef.reserve(FreqArray.Num());
		BandCoef.clear();
		std::vector<int> FreqBaund;
		FreqBaund.reserve(FreqArray.Num() * 2);
		for (auto& f : FreqArray)
		{
			FreqBaund.push_back(f.StartFreq);
			FreqBaund.push_back(f.StopFreq);
			BandCoef.push_back(f.Coef);
		}
		_systemAnalyzer->InitializeBeatDetector(FreqBaund);
		_systemAnalyzer->InitializeSpector(NumBars);
		BeatStoreAnalyzer.Init(false, FreqArray.Num());
		SpectorStoreAnalyzer.Init(0, NumBars);

		_systemPlayer->SetSound(_systemAnalyzer->GetSound());
		_systemPlayer->Play();
		_systemPlayer->Pause(true);
		_systemPlayer->InitializeBeatDetector(FreqBaund);
		_systemPlayer->InitializeSpector(NumBars);
		BeatStorePlayer.Init(false, FreqArray.Num());
		SpectorStorePlayer.Init(0, NumBars);
		isLoad = true;
	}
}

void UFMODManager::Play()
{
	if (_systemAnalyzer)
	{
		_systemAnalyzer->Pause(false);

		GetWorld()->GetTimerManager().SetTimer(Timer, this, &UFMODManager::PlaySound, Delay, false);
	}
}

void UFMODManager::PlaySound()
{
	if (_systemPlayer)
	{
		_systemPlayer->Pause(false);
	}
	GetWorld()->GetTimerManager().ClearTimer(Timer);
}

void UFMODManager::PauseAnalyze(bool IsPause)
{
	if (_systemAnalyzer)
	{
		_systemAnalyzer->Pause(IsPause);
	}
}

void UFMODManager::PausePlayer(bool IsPause)
{
	if (_systemPlayer)
	{
		_systemPlayer->Pause(IsPause);
	}
}

void UFMODManager::UpdateSystem()
{
	if (_systemAnalyzer)
	{
		_systemAnalyzer->SystemUpdate();
	}
	if (_systemPlayer)
	{
		_systemPlayer->SystemUpdate();
	}
}

void UFMODManager::Stop()
{
	if (_systemAnalyzer)
	{
		_systemAnalyzer->Stop();
	}
	if (_systemPlayer)
	{
		_systemPlayer->Stop();
	}
}

void UFMODManager::Tick(float DeltaTime)
{
	if (isLoad)
	{
		UpdateSystem();

		if (_systemAnalyzer)
		{
			_systemAnalyzer->CalculetaBeatDetector(BeatStoreAnalyzer.GetData(), BandCoef);
			_systemAnalyzer->CalculateSpector(SpectorStoreAnalyzer.GetData());
		}
		if (_systemPlayer)
		{
			_systemPlayer->CalculetaBeatDetector(BeatStorePlayer.GetData(), BandCoef);
			_systemPlayer->CalculateSpector(SpectorStorePlayer.GetData());
			SPM = _systemPlayer->GetSPM();
			PeakLevel = _systemPlayer->GetPeakSoundLevel();
		}
		if (_systemPlayer->GetSoundPosition() >= SoundLength)
		{
			DelegateOverSound.Broadcast();
			isLoad = false;
		}
	}
}

TStatId UFMODManager::GetStatId() const
{
	return UObject::GetStatID();
}

bool UFMODManager::IsTickable() const
{
	return true;
}

bool UFMODManager::IsTickableInEditor() const
{
	return false;
}

bool UFMODManager::IsTickableWhenPaused() const
{
	return false;
}
