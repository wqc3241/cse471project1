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
	m_envelope = new CADSR();
	// set the attack and release of the ADSR envelope
	static_cast<CADSR*>(m_envelope)->SetAttack(.05);
	static_cast<CADSR*>(m_envelope)->SetRelease(.05);

	// set the envelope of the filter
	m_amp_filter.SetEnvelope(m_envelope);
	// set the source to be the additive wave object
	m_amp_filter.SetSource(&m_sinewave);
	// set the sample rate
	m_amp_filter.SetSampleRate(GetSampleRate());
	// set the duration
	m_amp_filter.SetDuration(m_duration);
	m_amp_filter.Start();
}


bool CAdditiveInstrument::Generate()
{
	// begin envelope generation
	m_envelope->Generate();

	// if cross fade flag is true, implement cross fading
	if (m_sinewave.GetCrossfadeFlag())
	{
		// know when to end the cross fade effect
		auto crossfade_end_time = m_sinewave.GetCrossfadeStartTime() + m_crossfade_duration;

		if (m_time < m_sinewave.GetCrossfadeStartTime())
		{
			// generate wave by adding together several sinusoids (additive wave)
			m_sinewave.Generate();
		}

		else if (m_time > m_sinewave.GetCrossfadeStartTime() && m_time < crossfade_end_time)
		{
			// generate the cross fade wave
			m_sinewave.GenerateCrossfade(m_time, m_crossfade_duration);
		}
	}

	else
	{
		// generate wave by adding together several sinusoids (additive wave)
		m_sinewave.Generate();
	}

	// generate audio sample
	auto okay = m_amp_filter.Generate();

	// read the sample, make it the result frame
	m_frame[0] = m_amp_filter.Frame(0);
	m_frame[1] = m_amp_filter.Frame(1);

	// update the time
	m_time += GetSamplePeriod();

	// returns true until the time equals the duration
	return okay;
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
			// 1 / (m_bpm / 60) - seconds per beat
			// this prevents unwanted overlap and correct duration of each note
			SetDuration(value.dblVal * (1 / (m_bpm / 60)));
		}

		else if (name == "note")
		{
			SetFreq(NoteToFrequency(value.bstrVal));
		}

		else if (name == "harmonics")
		{
			AddHarmonics(value.bstrVal);
		}

		else if (name == "vibrato")
		{
			value.ChangeType(VT_R8);
			m_sinewave.SetVibratoFlag(true);
			m_sinewave.SetVibrato(value.dblVal);
		}

		else if (name == "vibratoRate")
		{
			value.ChangeType(VT_R8);
			m_sinewave.SetVibratoRate(value.dblVal);
		}

		else if (name == "crossfade")
		{
			value.ChangeType(VT_R8);

			m_crossfade_duration = value.dblVal;
			m_sinewave.SetCrossfadeFlag(true);
			auto start_time = m_duration - m_crossfade_duration;

			// set the time to start the cross fade
			m_sinewave.SetCrossfadeStartTime(start_time);
		}

		else if (name == "crossfadeOverlap")
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


void CAdditiveInstrument::AddHarmonics(wstring harmonics)
{
	// get the string stream of harmonics in the XML file
	// harmonics = 1 .5 .7
	wstringstream sstream(harmonics);
	// individual amplitude in list (like 1, .5, or .7)
	wstring harmonic_amp;

	// read into the wave
	while (sstream >> harmonic_amp)
	{
		// index - harmonic
		// value - amplitude
		// convert amplitude from string to double
		m_sinewave.AddHarmonic(stod(harmonic_amp));
	}
}