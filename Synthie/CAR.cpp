#include "stdafx.h"
#include "CAR.h"


CCAR::CCAR()
{
	m_attack = 0.05;
	m_release = 0.05;
	m_duration = 0.1;
}


CCAR::~CCAR()
{
}


void CCAR::Start()
{
	m_source->SetSampleRate(GetSampleRate());
	m_source->Start();
	m_time = 0;
}


bool CCAR::Generate()
{
	m_source->Generate();

	if ((m_duration - m_release) < m_time)
	{
		m_frame[0] = m_source->Frame(0) * (m_duration - m_time) / m_release;
		m_frame[1] = m_source->Frame(1) * (m_duration - m_time) / m_release;
	}

	else if (m_attack > m_time)
	{
		m_frame[0] = m_source->Frame(0) * m_time / m_attack;
		m_frame[1] = m_source->Frame(1) * m_time / m_attack;
	}

	else
	{
		m_frame[0] = m_source->Frame(0);
		m_frame[1] = m_source->Frame(1);
	}

	m_time += GetSamplePeriod();

	return m_duration > m_time;
}
