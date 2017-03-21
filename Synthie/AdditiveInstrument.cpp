#include "StdAfx.h"
#include "AdditiveInstrument.h"
#include "Notes.h"
#include <string>
#include <sstream>
#include <vector>



CAdditiveInstrument::CAdditiveInstrument()
{
	m_crossFadeIn = 0.0;
	m_crossFadeOut = 0.0;
	m_attack = 0.1;
	m_release = 0.1;
	m_decay = 0.05;
	m_sustain = 1.0;
}


CAdditiveInstrument::~CAdditiveInstrument()
{
}

void CAdditiveInstrument::Start()
{
	m_sinewave.SetSampleRate(GetSampleRate());
	m_sinewave.SetDuration(m_duration);
	m_sinewave.Start();
	m_time = 0;
}


bool CAdditiveInstrument::Generate()
{
	// Tell the component to generate an audio sample
	m_sinewave.Generate();

	// Read the component's sample and make it our resulting frame.


	m_frame[0] = m_sinewave.Frame(0);
	m_frame[1] = m_sinewave.Frame(1);

	//ATTACK AND RELEASE IMPLEMENATION
	/*double factor = 1.0;
	if(m_time < m_attack && m_time > (m_duration  - m_release))
	factor = m_time*1./m_attack < m_time*-1./m_release + (1./m_release)*(m_duration) ? m_time*-1./m_release + (1./m_release)*(m_duration) : m_time*1./m_attack;
	else if(m_time < m_attack)
	factor = m_time*1./m_attack;
	else if(m_time > (m_duration  - m_release))
	factor = m_time*-1./m_release + (1./m_release)*(m_duration);
	*/
	double ratio = 1.0;
	double sign = -1.0;

	if (m_sustain > 1.0) {
		sign = 1.0;
	}

	if (m_time < m_attack) {
		ratio = m_time*1. / m_attack;
	}

	else if (m_time < m_decay) {
		ratio = sign*((1.0 - m_sustain) / (m_decay - m_attack))*(m_time)+(1.0 - sign*((1.0 - m_sustain) / (m_decay - m_attack))*(m_attack));
	}

	else if (m_time >(m_duration - m_release) && m_release != 0) {
		if (m_sustain <= 0) m_sustain = 1.0;
		ratio = m_time*-m_sustain / m_release + (m_sustain / m_release)*(m_duration);
	}

	else {
		ratio = m_sustain;
	}

	m_frame[0] = m_frame[1] *= ratio/32767;

	// Update time
	m_time += GetSamplePeriod();

	// We return true until the time reaches the duration.
	return m_time < m_duration;
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
	for (int i = 0; i<len; i++)
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
			SetDuration(value.dblVal);
		}

		else if (name == "note")
		{
			SetFreq(NoteToFrequency(value.bstrVal));
		}
		
		else if (name == "amplitudes")
		{
			std::wstring dataset(value.bstrVal);
			std::string str(dataset.begin(), dataset.end());

			std::stringstream ss(str);
			std::string data;

			double harmonics[8] = { 0 };

			int i = 0;
			while (std::getline(ss, data, char(32))) {
				harmonics[i++] = atof(data.c_str());
			}

			SetAmplitude(harmonics[0]);
		}
		
		else if (name == "crossFade")
		{
			std::wstring dataset(value.bstrVal);
			std::string str(dataset.begin(), dataset.end());

			std::stringstream ss(str);
			std::string data;

			std::getline(ss, data, char(32));
			SetCrossFadeIn(atof(data.c_str()));

			std::getline(ss, data, char(32));
			SetCrossFadeOut(atof(data.c_str()));
		}
		
		else if (name == "vibrato") {
			std::wstring dataset(value.bstrVal);
			std::string str(dataset.begin(), dataset.end());

			std::stringstream ss(str);
			std::string data;

			std::getline(ss, data, char(32));
			m_sinewave.SetVibratoRate(atof(data.c_str()));

			std::getline(ss, data, char(32));
			m_sinewave.SetVibratoFreq(atof(data.c_str()));
		}

	}

}