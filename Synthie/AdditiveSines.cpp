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
	// set the phase
	m_phase = (m_phase + 1) % m_wavetable.size();

	return true;
}


void CAdditiveSines::SetWavetables()
{
	// set the table size to the sample rate
	auto table_size = GetSampleRate();
	// resize the wave table
	m_wavetable.resize(table_size);

	// sine and vibrato radians
	double sine_rads = 0;
	double vibrato_rads = 0;

	for (auto i = 0; i < table_size; i++, m_time += 1. / GetSampleRate())
	{
		// set the sample
		auto sample = sin(sine_rads) * m_amp;

		// check if there are harmonics
		if (m_harmonics.size() > 0)
		{
			ImplementHarmonics(&sample, m_time);
		}

		// check for vibrato effect
		if (m_implement_vibrato)
		{
			ImplementVibrato(&sine_rads, &vibrato_rads);
		}

		// set each index in the wave table to the sample
		m_wavetable[i] = sample;
	}
}


void CAdditiveSines::ImplementHarmonics(double* sample, double time)
{
	// Nyquist Frequency = sample rate / 2
	auto nyquist_freq = GetSampleRate() / 2;

	// go until the end of the harmonics (vector)
	for (auto x = 0; x < m_harmonics.size() && (m_freq * (x + 1)) < nyquist_freq; x++)
	{
		// adjust to match the harmonic number
		auto harmonic = x + 1;

		// save value of sample in memory to be used at end of SetWavetables()
		*sample += m_harmonics[x] * (m_amp / harmonic) * (sin(time * 2 * PI * harmonic * m_freq));
	}
}


void CAdditiveSines::ImplementVibrato(double* sine_rads, double* vibrato_rads)
{
	// sine wave
	double sample = short(m_amp * sin(*sine_rads));
	// rate
	double diff = sin(*vibrato_rads) * m_vibrato_effect.vibrato;

	// increment phases
	// save the sine radians and vibrato radians in memory
	*sine_rads += (2 * PI * (m_freq + diff)) / m_sampleRate;
	*vibrato_rads += (2 * PI * m_vibrato_effect.vibrato_rate) / m_sampleRate;
}


void CAdditiveSines::GenerateCrossfade(double time, double crossfade_dur)
{
	// time of the cross fade elapsed
	auto elapsed_time = time - m_crossfade_start_time;
	// next sample
	double next_sample[2];
	// current sample
	double current_sample[2];
	// in-between sample
	double interpolated_sample;

	if (m_next_wave != nullptr && m_phase < m_next_wave->GetWavetableSize() && m_phase < GetWavetableSize())
	{
		// next
		next_sample[0] = next_sample[1] = m_next_wave->m_wavetable[m_phase];
		// current
		current_sample[0] = current_sample[1] = m_wavetable[m_phase];
		// in-between
		interpolated_sample = current_sample[0] * ((crossfade_dur - elapsed_time) / crossfade_dur) +
			next_sample[0] * (elapsed_time / crossfade_dur);

		// update frame
		m_frame[0] = m_frame[1] = interpolated_sample;
	}

	else
	{
		// update frames
		m_frame[0] = m_wavetable[m_phase];
		m_frame[1] = m_frame[0];
	}

	// set the phase
	m_phase = (m_phase + 1) % m_wavetable.size();
}