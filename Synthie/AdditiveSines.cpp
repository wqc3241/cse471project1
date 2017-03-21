#include "StdAfx.h"
#include "AdditiveSines.h"
#include <iostream>
#include <cmath>

CAdditiveSines::CAdditiveSines()
{
	m_vFreq = 0.0;
	m_vRate = 0.0;
	m_amplitude = 1000.0;
	m_amp[0] = 1.0;
	for (int i = 1; i <= 7; i++) m_amp[i] = 0.0;
	m_freq = 440.0;

}


CAdditiveSines::~CAdditiveSines()
{
	delete[] audio;
	audio = NULL;
}

void CAdditiveSines::Start()
{
	audio = new short[int(m_duration*GetSampleRate())];
	index = 0;
	GenerateWaveTable();
}

bool CAdditiveSines::Generate()
{
	m_frame[1] = m_frame[0] = audio[index++];

	return true;
}

void CAdditiveSines::GenerateWaveTable()
{
	double sineRadians = 0;
	double vibratoRadians = 0;

	int nyquist = GetSampleRate() / 2;

	double time = 0; int i;
	for (i = 0; i < int(m_duration*GetSampleRate()); i++, time += 1. / GetSampleRate())
	{
		double sample = 0;
		
		for (float n = 1.0; n*m_freq <= nyquist && n <= 8.0; n++) {
			sample += (m_amplitude * m_amp[int(n - 1)] * (sin(n * sineRadians)));
		}

		vibratoRadians += (2 * PI * m_vRate) / GetSampleRate();
		sineRadians += (2 * PI * (m_freq + m_vFreq * sin(vibratoRadians))) / GetSampleRate();

		audio[i] = sample;
	}
}