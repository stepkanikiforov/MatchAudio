#include "FMODSystem.h"
#include <cmath>

FMODSystem::FMODSystem() : _system(NULL),
_channel(NULL),
_sound(NULL),
_dsp(NULL),
_mdsp(NULL),
_windowSize(1024),
_samplingFrequency(0),
_FFThistory_MaxSize(0)
{

}

FMODSystem::~FMODSystem()
{
	ReleaseSystem();
}

void FMODSystem::InitializeSystem(bool IsOutput)
{
	FMOD::System_Create(&_system);
	if (_system)
	{
		if (!IsOutput)
		{
			_system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		}
		_system->init(1, FMOD_INIT_NORMAL, NULL);
		_system->createDSPByType(FMOD_DSP_TYPE_FFT, &_dsp);
		if (_dsp)
		{
			_dsp->setParameterInt(FMOD_DSP_FFT_WINDOWTYPE, FMOD_DSP_FFT_WINDOW_HANNING);
			_dsp->setParameterInt(FMOD_DSP_FFT_WINDOWSIZE, _windowSize);
		}
	}
}

void FMODSystem::LoadSoundFromMemory(char* MemoryPtr, unsigned int MemorySize)
{
	FMOD_CREATESOUNDEXINFO sndinfo = { 0 };
	sndinfo.cbsize = sizeof(sndinfo);
	sndinfo.length = MemorySize;

	if (_system)
	{
		_system->createSound(MemoryPtr, FMOD_OPENMEMORY | FMOD_DEFAULT | FMOD_CREATESAMPLE, &sndinfo, &_sound);
		//_sound->getLength(&_soundLength, FMOD_TIMEUNIT_MS);
	}
}

void FMODSystem::Play()
{
	if (_system && _sound)
	{
		_system->playSound(_sound, 0, false, &_channel);
		if (_channel)
		{
			_channel->getFrequency(&_samplingFrequency);
			_FFThistory_MaxSize = _samplingFrequency / _windowSize;
			if (_dsp)
			{
				_channel->addDSP(0, _dsp);
				_dsp->setActive(true);
			}
			_channel->getDSP(FMOD_CHANNELCONTROL_DSP_HEAD, &_mdsp);
			if (_mdsp)
			{
				_mdsp->setMeteringEnabled(false, true);
			}
		}
	}
}

void FMODSystem::SystemUpdate()
{
	if (_system)
	{
		_system->update();
	}
}

void FMODSystem::Pause(bool IsPause)
{
	if (_channel)
	{
		_channel->setPaused(IsPause);
	}
}

void FMODSystem::Stop()
{
	if (_channel)
	{
		_channel->stop();
	}
	if (_dsp)
	{
		_dsp->release();
	}
	if (_mdsp)
	{
		_mdsp->release();
	}
	if (_sound)
	{
		_sound->release();
	}
}

void FMODSystem::ReleaseSystem()
{
	if (_dsp)
	{
		_dsp->release();
	}
	if (_mdsp)
	{
		_mdsp->release();
	}
	if (_sound)
	{
		_sound->release();
	}
	if (_system)
	{
		_system->close();
		_system->release();
	}
}

FMOD::Sound* FMODSystem::GetSound() const
{
	if (_sound)
	{
		return _sound;
	}
	return NULL;
}

void FMODSystem::SetSound(FMOD::Sound* Sound)
{
	_sound = Sound;
}

float FMODSystem::GetPeakSoundLevel() const
{
	if (_mdsp)
	{
		FMOD_DSP_METERING_INFO peaklevels = {};
		_mdsp->getMeteringInfo(0, &peaklevels);
		if (peaklevels.numsamples > 0)
		{
			return peaklevels.peaklevel[0];
		}
		return 0;
	}
	return 0;
}

float FMODSystem::GetSPM() const
{
	return SPM;
}

int FMODSystem::GetSoundLendth()
{
	//return _soundLength;

	_sound->getLength(&_soundLength, FMOD_TIMEUNIT_MS);
	return _soundLength;
}

int FMODSystem::GetSoundPosition()
{
	unsigned int position = 0;
	_channel->getPosition(&position, FMOD_TIMEUNIT_MS);
	return position;
}

bool FMODSystem::IsSoundOver()
{
	unsigned int position = 0;
	_channel->getPosition(&position, FMOD_TIMEUNIT_MS);
	if (position >= _soundLength)
	{
		return true;
	}
	return false;
}

void FMODSystem::InitializeLogSpector(int NumBars)
{
	std::vector<int> frequencyOctaves;
	frequencyOctaves.push_back(0);
	for (int i = 1; i < 13; ++i)
	{
		frequencyOctaves.push_back((int)((_samplingFrequency / 2) / (float)pow(2, 12 - i)));
	}

	int bandsPerOctave = NumBars / 12;

	if (_FFThistory_MaxSize != 0)
	{
		_numSamplesPerBar_log.clear();
		for (int octave = 0; octave < 12; ++octave)
		{
			int indexLow = frequencyOctaves[octave] / _FFThistory_MaxSize;
			int indexHigh = (frequencyOctaves[octave + 1]) / _FFThistory_MaxSize;
			int octavaIndexes = (indexHigh - indexLow);

			if (octavaIndexes > 0)
			{
				if (octavaIndexes <= bandsPerOctave)
				{
					for (int count = 0; count < octavaIndexes; ++count)
					{
						_numSamplesPerBar_log.push_back(1);
					}
				}
				else
				{
					for (int count = 0; count < bandsPerOctave; ++count)
					{
						_numSamplesPerBar_log.push_back(octavaIndexes / bandsPerOctave);
					}
				}
			}
		}
	}
}

void FMODSystem::InitializeSpector(int NumBars)
{
	float extent = 0;
	extent = std::log2f((float)(_windowSize / 2)) / std::log2f((float)NumBars);

	_numSamplesPerBar_log.clear();
	_numSamplesPerBar_log.reserve(NumBars);
	_numSamplesPerBar_log.push_back(1);
	for (int i = 1; i < NumBars; ++i)
	{
		_numSamplesPerBar_log.push_back((int)std::pow(i, extent) - (int)std::pow(i - 1, extent));
	}

}

void FMODSystem::InitializeBeatDetector(std::vector<int>& StoreFreq)
{
	_beatDetector_bandLimits.clear();
	_beatDetector_bandLimits.reserve(StoreFreq.size());
	if (_FFThistory_MaxSize != 0)
	{
		for (int i = 0; i < StoreFreq.size(); i = i + 2)
		{
			_beatDetector_bandLimits.push_back(StoreFreq[i] / _FFThistory_MaxSize);
			_beatDetector_bandLimits.push_back(StoreFreq[i + 1] / _FFThistory_MaxSize);
		}
		_FFTHistory_beatDetector.clear();
	}
}

void FMODSystem::CalculateSpector(float* FillStoreSpector)
{
	FMOD_DSP_PARAMETER_FFT* dspFFT = NULL;
	if (_dsp)
	{
		_dsp->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&dspFFT, 0, 0, 0);
	}

	if (dspFFT)
	{
		int length = dspFFT->length / 2;
		int numChannels = dspFFT->numchannels;

		if (length > 0)
		{
			int indexFFT = 0;
			for (int index = 0; index < _numSamplesPerBar_log.size(); ++index)
			{
				FillStoreSpector[index] = 0;
				for (int frec = 0; frec < _numSamplesPerBar_log[index]; ++frec)
				{
					for (int channel = 0; channel < numChannels; ++channel)
					{
						FillStoreSpector[index] += dspFFT->spectrum[channel][indexFFT];
					}
					++indexFFT;
				}
				FillStoreSpector[index] /= (float)(_numSamplesPerBar_log[index] * numChannels);
			}
		}
	}
}

void FMODSystem::CalculetaBeatDetector(bool* StoreBeat, std::vector<float>& StoreCoef)
{
	FMOD_DSP_PARAMETER_FFT* dspFFT = NULL;
	if (_dsp)
	{
		_dsp->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&dspFFT, 0, 0, 0);
	}

	if (dspFFT)
	{
		int length = dspFFT->length / 2;
		int numChannels = dspFFT->numchannels;

		if (length > 0)
		{
			int numBands = _beatDetector_bandLimits.size() / 2;
			std::vector<float> spectrum(numBands, 0);

			for (int numBand = 0; numBand < numBands; ++numBand)
			{
				StoreBeat[numBand] = false;
				int bandBoundIndex = numBand * 2;
				for (int indexFFT = _beatDetector_bandLimits[bandBoundIndex]; indexFFT < _beatDetector_bandLimits[bandBoundIndex + 1]; ++indexFFT)
				{
					for (int channel = 0; channel < numChannels; ++channel)
					{
						spectrum[numBand] += dspFFT->spectrum[channel][indexFFT];
					}
				}
				spectrum[numBand] /= (_beatDetector_bandLimits[bandBoundIndex + 1] - _beatDetector_bandLimits[bandBoundIndex]) * numChannels;
			}

			if (_FFTHistory_beatDetector.size() > 0)
			{
				std::vector<float> averageSpectrum(numBands, 0);
				fillAverageSpectrum(averageSpectrum, _FFTHistory_beatDetector);

				std::vector<float> varianceSpectrum(numBands, 0);
				fillVarianceSpectrum(varianceSpectrum, _FFTHistory_beatDetector, averageSpectrum);

				for (int i = 0; i < numBands; ++i)
				{
					StoreBeat[i] = (spectrum[i] - StoreCoef[i]) > beatThreshold(varianceSpectrum[i]) * averageSpectrum[i];
				}
			}

			if (_FFTHistory_beatDetector.size() >= _FFThistory_MaxSize)
			{
				_FFTHistory_beatDetector.pop_front();
			}

			_FFTHistory_beatDetector.push_back(spectrum);
		}
	}
}

void FMODSystem::fillAverageSpectrum(std::vector<float>& averageSpectrum, const FFTHistoryContainer& fftHistory)
{
	for (const auto& h : fftHistory)
	{
		for (int i = 0; i < h.size(); ++i)
		{
			averageSpectrum[i] += h[i];
		}
	}
	for (int i = 0; i < averageSpectrum.size(); ++i)
	{
		averageSpectrum[i] /= fftHistory.size();
	}
}

void FMODSystem::fillVarianceSpectrum(std::vector<float>& varianceSpectrum, const FFTHistoryContainer& fftHistory, const std::vector<float>& averageSpectrum)
{
	for (const auto& h : fftHistory)
	{
		for (int i = 0; i < h.size(); ++i)
		{
			varianceSpectrum[i] += (h[i] - averageSpectrum[i]) * (h[i] - averageSpectrum[i]);
		}
	}
	for (int i = 0; i < varianceSpectrum.size(); ++i)
	{
		varianceSpectrum[i] /= fftHistory.size();
	}
}

float FMODSystem::beatThreshold(float variance)
{
	return -15 * variance + 1.55;
}
