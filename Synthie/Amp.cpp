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

	m_ADSR->SetDuration(m_duration);
	m_ADSR->SetSampleRate(GetSampleRate());

	// start envelope generation
	m_ADSR->Start();
}


bool CAmp::Generate()
{
	// get the amplitude factor, based on envelope level
	auto amp_factor = m_ADSR->GetEnvelopeLevel();

	// set the frames 
	m_frame[0] = m_source->Frame(0) * amp_factor;
	m_frame[1] = m_source->Frame(1) * amp_factor;

	// update the time
	m_time += GetSamplePeriod();

	return m_duration > m_time;
}