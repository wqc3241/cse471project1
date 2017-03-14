#include "StdAfx.h"
#include "AdditiveSines.h"
#include <iostream>
#include <cmath>

short SoundRange(double d)
{
	if (d < -32768)
		return -32768;
	if (d > 32767)
		return 32767;
	return short(d);
}

CAdditiveSines::CAdditiveSines(void)
{
	m_vFreq = 0.0;
	m_vRate = 0.0;
	m_amplitude = 1000.0;
	m_amp[8] = { 1.0 };
	m_freq = 440.0;
}


CAdditiveSines::~CAdditiveSines(void)
{
	delete[] audio;
	audio = NULL;
}

void CAdditiveSines::Start()
{
	audio = new short[int(m_duration * GetSampleRate())];
	index = 0;
	GenerateWaveTable();
}

bool CAdditiveSines::Generate()
{
	m_frame[0] = audio[index++];
	m_frame[1] = m_frame[0];
	return true;
}

void CAdditiveSines::GenerateWaveTable()
{

	double sineRadians = 0;
	double vibratoRadians = 0;

	double time = 0;
	for (int i = 0; i < int(m_duration*GetSampleRate()); i++, time += 1. / GetSampleRate())
	{
		double sample = 0;
		
		float n = 1.0;
		while(n * m_freq <= 22050 && n <= 8.0) {
			sample += (m_amplitude * m_amp[int(n - 1)] * (sin(n * sineRadians)));
			n += 1;
		}

		vibratoRadians += (2 * PI * m_vRate) / GetSampleRate();
		sineRadians += (2 * PI * (m_freq + m_vFreq * sin(vibratoRadians))) / GetSampleRate();

		audio[i] = SoundRange(sample);
	}
}