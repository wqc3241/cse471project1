#include "stdafx.h"
#include "Amp.h"


CAmp::CAmp()
{
}


CAmp::~CAmp()
{
}

void CAmp::Start()
{
	m_time = 0;

	// set the duration, sample rate of the envelope
	m_envelope->SetDuration(m_duration);
	m_envelope->SetSampleRate(GetSampleRate());

	// start envelope generation
	m_envelope->Start();
}


bool CAmp::Generate()
{
	// get the amplitude factor, based on envelope level
	auto amplitude_factor = m_envelope->GetEnvelopeLevel();

	// set the frames 
	m_frame[0] = m_source->Frame(0) * amplitude_factor;
	m_frame[1] = m_source->Frame(1) * amplitude_factor;

	// update the time
	m_time += GetSamplePeriod();

	return m_duration > m_time;
}