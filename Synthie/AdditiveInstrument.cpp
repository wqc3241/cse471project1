#include "stdafx.h"
#include "AdditiveInstrument.h"
#include "Note.h"
#include "Notes.h"
#include "ADSR.h"
#include <sstream>


CAdditiveInstrument::CAdditiveInstrument()
{
	m_duration = 0.1;
}


CAdditiveInstrument::CAdditiveInstrument(double bpm)
{
	m_duration = 0.1;
	m_bpm = bpm;
}


CAdditiveInstrument::~CAdditiveInstrument()
{
}


void CAdditiveInstrument::Start()
{
	// set the sample rate of the wave
	m_sinewave.SetSampleRate(GetSampleRate());
	m_sinewave.Start();
	m_time = 0;

	// create a new envelope object
	m_ADSR = new CADSR();
	m_ADSR->SetAttack(.05);
	m_ADSR->SetRelease(.05);

	m_amp.SetEnvelope(m_ADSR);
	m_amp.SetSource(&m_sinewave);
	m_amp.SetSampleRate(GetSampleRate());
	m_amp.SetDuration(m_duration);
	m_amp.Start();
}


bool CAdditiveInstrument::Generate()
{
	// envelope generation
	m_ADSR->Generate();

	// implement cross fading
	if (m_sinewave.GetCrossfadeFlag())
	{
		//end cross fade effect
		auto crossfade_end_time = m_sinewave.GetCrossfadeStartTime() + m_crossFade;

		if (m_time < m_sinewave.GetCrossfadeStartTime())
		{
			m_sinewave.Generate();
		}

		else if (m_time > m_sinewave.GetCrossfadeStartTime() && m_time < crossfade_end_time)
		{
			m_sinewave.GenerateCrossfade(m_time, m_crossFade);
		}
	}

	else
	{
		m_sinewave.Generate();
	}

	// generate sample
	bool valid = m_amp.Generate();

	// read sample, make it the result frame
	m_frame[0] = m_amp.Frame(0);
	m_frame[1] = m_amp.Frame(1);

	// update the time
	m_time += GetSamplePeriod();

	// returns true until the time equals the duration
	return valid;
}


void CAdditiveInstrument::SetNote(CNote *note)
{
	// Get a list of all attribute nodes and the
	// length of that list
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	note->Node()->get_attributes(&attributes);
	long len;
	attributes->get_length(&len);

	// Loop over the list of attributes
	for (int i = 0; i < len; i++)
	{
		// Get attribute i
		CComPtr<IXMLDOMNode> attrib;
		attributes->get_item(i, &attrib);

		// Get the name of the attribute
		CComBSTR name;
		attrib->get_nodeName(&name);

		// Get the value of the attribute.  A CComVariant is a variable
		// that can have any type. It loads the attribute value as a
		// string (UNICODE), but we can then change it to an integer 
		// (VT_I4) or double (VT_R8) using the ChangeType function 
		// and then read its integer or double value from a member variable.
		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == "duration")
		{
			value.ChangeType(VT_R8);
			SetDuration(value.dblVal * (60 / m_bpm));
		}

		else if (name == "note")
		{
			SetFreq(NoteToFrequency(value.bstrVal));
		}

		else if (name == "harmonics")
		{
			value.ChangeType(VT_I2);
			m_sinewave.SetHarmonics(value.lVal);
		}

		else if (name == "vibrato")
		{

			std::wstring vibrato(value.bstrVal);
			std::string data(vibrato.begin(), vibrato.end());

			std::stringstream ss(data);
			std::string item;

			m_sinewave.SetVibratoFlag(true);

			std::getline(ss, item, char(32));
			m_sinewave.SetVibrato(atof(item.c_str()));

			std::getline(ss, item, char(32));
			m_sinewave.SetVibratoRate(atof(item.c_str()));
		}

		else if (name == "crossfade")
		{
			value.ChangeType(VT_R8);

			m_crossFade = value.dblVal;
			m_sinewave.SetCrossfadeFlag(true);
			auto start_time = m_duration - m_crossFade;

			// set the time to start the cross fade
			m_sinewave.SetCrossfadeStartTime(start_time);
		}

		else if (name == "crossfadecross")
		{
			// overlap in beats
			value.ChangeType(VT_R8);

			// 1 / (m_bpm / 60) - seconds per beat
			auto overlap_time = (1 / (m_bpm / 60)) * value.dblVal;
			auto crossfade_time = m_sinewave.GetCrossfadeStartTime();

			// set the time to start the cross fade, taking into account the overlap time
			m_sinewave.SetCrossfadeStartTime(crossfade_time - overlap_time);
		}
	}
}


void CAdditiveInstrument::SetNextNote(CNote* next_note)
{
	// used for cross fade
	m_next = new CAdditiveInstrument();

	// set the sample rate
	m_next->SetSampleRate(GetSampleRate());
	// move to the next note
	m_next->SetNote(next_note);
	m_next->Start();

	// used for interpolation between sound A and B
	m_sinewave.SetNextWave(&m_next->m_sinewave);
}