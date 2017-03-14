#pragma once
#include "AudioNode.h"
class CAdditiveSines :
	public CAudioNode
{
public:
	CAdditiveSines(void);
	~CAdditiveSines(void);
public:
	//! Start audio generation
	virtual void Start();

	//! Generate one frame of audio
	virtual bool Generate();

	void SetFreq(double f) { m_freq = f; }

	void SetAmplitude(double &a) { memcpy(&m_amp, &a, sizeof(double) * 8); }
	void SetDuration(double d) { m_duration = d; }
	void SetVibratoRate(double d) { m_vRate = d; }
	void SetVibratoFreq(double d) { m_vFreq = d; }

	void GenerateWaveTable();

private:
	double m_duration;
	double m_freq;
	int index;
	
	short *audio;
	
	double m_amplitude;
	double m_amp[8];

	double m_vRate;
	double m_vFreq;
};



