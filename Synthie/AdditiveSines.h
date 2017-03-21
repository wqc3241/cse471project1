#pragma once
#include "AudioNode.h"
class CAdditiveSines :
	public CAudioNode
{
public:
	CAdditiveSines();
	~CAdditiveSines();
public:
	//! Start audio generation
	virtual void Start();

	//! Generate one frame of audio
	virtual bool Generate();

	//! Set the sine wave frequency
	void SetFreq(double f) { m_freq = f; }

	//! Set the sine wave amplitude
	void SetAmplitude(double &a) { memcpy(&m_amp, &a, sizeof(double) * 8); }

	void SetDuration(double d) { m_duration = d; }

	void SetVibratoRate(double r) { m_vRate = r; }

	void SetVibratoFreq(double f) { m_vFreq = f; }

	void GenerateWaveTable();

private:
	double m_duration;
	double m_freq;
	double m_amp[8];
	double m_amplitude;
	int index;
	short *audio;
	double m_vRate;
	double m_vFreq;
};

