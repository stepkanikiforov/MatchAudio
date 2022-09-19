#pragma once
#include <vector>
#include <deque>
#include "fmod.hpp"

namespace FMOD
{
	class System;
	class Channel;
	class Sound;
	class DSP;
};

class FMODSystem
{
public:
	FMODSystem();
	~FMODSystem();

	void InitializeSystem(bool IsOutput);
	void LoadSoundFromMemory(char* MemoryPtr, unsigned int MemorySize);

	void Play();
	void Pause(bool IsPause);
	void Stop();
	void SystemUpdate();

	void SetSound(FMOD::Sound* Sound);
	FMOD::Sound* GetSound() const;
	float GetPeakSoundLevel() const;
	float GetSPM() const;

	void InitializeLogSpector(int NumBars);
	void InitializeSpector(int NumBars);
	void InitializeBeatDetector(std::vector<int>& StoreFreq);
	void CalculateSpector(float* FillStoreSpector);
	void CalculetaBeatDetector(bool* StoreBeat, std::vector<float>& StoreCoef);
	int GetSoundLendth();
	int GetSoundPosition();
	bool IsSoundOver();

private:
	void ReleaseSystem();

	/*Main variables of system*/
	FMOD::System* _system;
	FMOD::Channel* _channel;
	FMOD::Sound* _sound;
	FMOD::DSP* _dsp;
	FMOD::DSP* _mdsp;

	/*Variables for FFT work*/
	typedef std::deque<std::vector<float>> FFTHistoryContainer;
	FFTHistoryContainer _FFTHistory_beatDetector;
	std::vector<int> _numSamplesPerBar_log;
	std::vector<int> _beatDetector_bandLimits;
	int _windowSize;
	float _samplingFrequency;
	int _FFThistory_MaxSize;
	float SPM = 0;
	unsigned int _soundLength = 0;

	/*Functions for FFT work*/
	void fillAverageSpectrum(std::vector<float>& averageSpectrum, const FFTHistoryContainer& fftHistory);
	void fillVarianceSpectrum(std::vector<float>& varianceSpectrum, const FFTHistoryContainer& fftHistory, const std::vector<float>& averageSpectrum);
	float beatThreshold(float variance);
};