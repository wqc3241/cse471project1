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

		if (m_harmonics.size() > 0)
		{
			auto nyquist_freq = GetSampleRate() / 2;

			// loop to the end of the harmonic
			for (auto n = 0; n < m_harmonics.size() && (m_freq * (n + 1)) < nyquist_freq; n++)
			{
				// adjust harmonic number
				auto harmonic = n + 1;

				// save value of sample in memory to be used at end of SetWavetables()
				sample += m_harmonics[n] * (m_amp / harmonic) * (sin(m_time * 2 * PI * harmonic * m_freq));
			}
		}

		// check for vibrato effect
		if (m_implement_vibrato)
		{
			double sample = short(m_amp * sin(sine_rads));
			double diff = sin(vibrato_rads) * m_vibrato;

			// increment phases
			// save the sine radians and vibrato radians
			sine_rads += (2 * PI * (m_freq + diff)) / m_sampleRate;
			vibrato_rads += (2 * PI * m_vibratoRate) / m_sampleRate;
		}

		// set each index in the wave table to the sample
		m_wavetable[i] = sample;
	}
}

void CAdditiveSines::GenerateCrossfade(double time, double crossfade_dur)
{
	// time of the cross fade elapsed
	auto elapsed_time = time - m_crossfade_start_time;
	double next_sample[2];
	double current_sample[2];
	double interpolated_sample;

	if (m_next_wave != nullptr && m_phase < m_next_wave->GetWavetableSize() && m_phase < GetWavetableSize())
	{
		next_sample[0] = next_sample[1] = m_next_wave->m_wavetable[m_phase];
		current_sample[0] = current_sample[1] = m_wavetable[m_phase];
		interpolated_sample = current_sample[0] * ((crossfade_dur - elapsed_time) / crossfade_dur) +
			next_sample[0] * (elapsed_time / crossfade_dur);

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