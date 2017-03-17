#include "stdafx.h"
#include "AdditiveSines.h"
#include <cmath>


CAdditiveSines::CAdditiveSines()
{
	m_phase = 0;
	m_amp = 0.1;
	m_freq = 440;
}


CAdditiveSines::~CAdditiveSines()
{
}


void CAdditiveSines::Start()
{
	m_phase = 0;
	m_time = 0;
	SetWavetables();
}


bool CAdditiveSines::Generate()
{
	// set the frames
	m_frame[0] = m_wavetable[m_phase];
	m_frame[1] = m_frame[0];

	m_phase = (m_phase + 1) % m_wavetable.size();

	return true;
}


void CAdditiveSines::SetWavetables()
{
	auto table_size = GetSampleRate();
	m_wavetable.resize(table_size);

	double sine_rads = 0;
	double vibrato_rads = 0;

	for (auto i = 0; i < table_size; i++, m_time += 1. / GetSampleRate())
	{
		// set the sample
		auto sample = sin(sine_rads) * m_amp;

		// how many harmonics want
		if (m_harmonics && m_harmonics > 0)
		{
			auto nyquist_freq = GetSampleRate() / 2;

			for (int h = 1; (m_freq * h) < (nyquist_freq); h++) {
				sample += (m_amp / m_harmonics) * (sin(m_time * 2 * PI * m_freq * m_harmonics));
			}
		}

		// check for vibrato effect
		if (m_implement_vibrato)
		{
			double sample = short(m_amp * sin(sine_rads));
			double diff = sin(vibrato_rads) * m_vibrato;

			// increment phases
			sine_rads += (2 * PI * (m_freq + diff)) / m_sampleRate;
			vibrato_rads += (2 * PI * m_vibratoRate) / m_sampleRate;
		}

		// set each index in the wave table to the sample
		m_wavetable[i] = sample;
	}
}

void CAdditiveSines::GenerateCrossfade(double time, double crossFadeDuration)
{
	// time of the cross fade elapsed
	auto medianTime = time - m_crossFadeStart;
	double next_sample[2];
	double current_sample[2];
	double interpolated_sample;

	if (m_nextWave != nullptr && m_phase < m_nextWave->GetWavetableSize() && m_phase < GetWavetableSize())
	{
		next_sample[0] = next_sample[1] = m_nextWave->m_wavetable[m_phase];
		current_sample[0] = current_sample[1] = m_wavetable[m_phase];
		interpolated_sample = current_sample[0] * ((crossFadeDuration - medianTime) / crossFadeDuration) +
			next_sample[0] * (medianTime / crossFadeDuration);

		// update frame
		m_frame[0] = m_frame[1] = interpolated_sample;
	}

	else
	{
		m_frame[0] = m_wavetable[m_phase];
		m_frame[1] = m_frame[0];
	}

	m_phase = (m_phase + 1) % m_wavetable.size();
}