#include "stdafx.h"
#include "WaveInstrumentFactory.h"
#include <cmath>

using namespace std;

CWaveInstrumentFactory::CWaveInstrumentFactory(void)
{
	m_fade = 0.5;
	m_attack = 0.05;
	m_release = 0.05;
}

CWaveInstrumentFactory::~CWaveInstrumentFactory(void)
{
}


CWaveInstrument *CWaveInstrumentFactory::CreateInstrument()
{
	CWaveInstrument *instrument = new CWaveInstrument();
	instrument->GetPlayer()->SetSamples(&m_wave[0], (int)m_wave.size());

	return instrument;
}

void CWaveInstrumentFactory::SetNote(CNote *note)
{
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	note->Node()->get_attributes(&attributes);
	long len;
	attributes->get_length(&len);

	for (int i = 0; i<len; i++)
	{
		CComPtr<IXMLDOMNode> attrib;
		attributes->get_item(i, &attrib);

		CComBSTR name;
		attrib->get_nodeName(&name);

		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == "file")
		{
			value.ChangeType(VT_BSTR);
			char filename[100];
			wcstombs(filename, value.bstrVal, 100);
			LoadFile(filename);
		}
		else if (name == "duration")
		{
			value.ChangeType(VT_R8);
			SetDuration(value.dblVal);
		}
		else if (name == "attack")
		{
			value.ChangeType(VT_R8);
			SetAttack(value.dblVal);
		}
		else if (name == "release")
		{
			value.ChangeType(VT_R8);
			SetRelease(value.dblVal);
		}
		else if (name == "pitch")
		{
			value.ChangeType(VT_R8);
			ChangePitch(value.dblVal);
		}
		else if (name == "fade")
		{
			value.ChangeType(VT_R8);
			SetFade(value.dblVal);
		}
		else if (name == "crossfade")
		{
			value.ChangeType(VT_BSTR);
			char filename[100];
			wcstombs(filename, value.bstrVal, 100);
			CrossFade(filename);
		}
	}

	ChangeDuration();
	Envelope();
}


bool CWaveInstrumentFactory::LoadFile(const char *filename)
{
	m_wave.clear();

	CDirSoundSource m_file;
	if (!m_file.Open(filename))
	{
		CString msg = L"Unable to open audio file: ";
		msg += filename;
		AfxMessageBox(msg);
		return false;
	}

	for (int i = 0; i<m_file.NumSampleFrames(); i++)
	{
		short frame[2];
		m_file.ReadFrame(frame);
		m_wave.push_back(frame[0]);
	}

	m_file.Close();
	return true;
}

void CWaveInstrumentFactory::ChangePitch(double rate)
{
	double sample = 0.0;
	vector<short> p_wave;

	if (rate == 1.0)
	{
		return;
	}

	while (sample < m_wave.size() - 1)
	{
		double	inbetween = fmod(sample, 1);
		short output = (1. - inbetween) * m_wave[int(sample)] + inbetween * m_wave[int(sample) + 1];

		p_wave.push_back(output);

		sample += rate;
	}

	m_wave = p_wave;
}

void CWaveInstrumentFactory::CrossFade(const char *filename)
{
	vector<short> f_wave;

	CDirSoundSource f_file;
	if (!f_file.Open(filename))
	{
		CString msg = L"Unable to open audio file: ";
		msg += filename;
		AfxMessageBox(msg);
		return;
	}

	for (int i = 0; i<f_file.NumSampleFrames(); i++)
	{
		short frame[2];
		f_file.ReadFrame(frame);
		f_wave.push_back(frame[0]);
	}

	f_file.Close();

	double output;
	int m_sample = 0;
	int o_sample = 0;
	double m_duration = m_wave.size() / 44100.;
	double fade_start = m_duration - m_fade;
	int f_sample = 0;
	double time = 0.0;
	vector<short> o_wave;

	while (f_sample < f_wave.size())
	{
		if (time < fade_start)
		{
			o_wave.push_back(m_wave[m_sample]);
			m_sample++;
			o_sample++;
			time += 1 / 44100.;
		}
		else if (time < (fade_start + m_fade))
		{
			if (m_sample > m_wave.size() - 1)
			{
				m_sample = m_wave.size() - 1;
			}
			output = (1 - ((time - fade_start) / m_fade))*m_wave[m_sample] + ((time - fade_start) / m_fade)*f_wave[f_sample];
			o_wave.push_back(short(output));

			m_sample++;
			o_sample++;
			f_sample++;
			time += 1 / 44100.;
		}
		else
		{
			o_wave.push_back(f_wave[f_sample]);
			o_sample++;
			f_sample++;
		}
	}

	m_wave = o_wave;
}

void CWaveInstrumentFactory::ChangeDuration()
{
	vector<short> o_wave;
	double m_time = 0.0;
	double length = m_wave.size() / 44100.;
	int i = 0;

	if (m_duration == 0)
	{
		return;
	}

	while (m_time < m_duration)
	{
		o_wave.push_back(m_wave[i]);

		i++;
		m_time += 1 / 44100.;

		if (i > m_wave.size() - 1)
		{
			i = 0;
		}
	}

	m_wave = o_wave;
}

void CWaveInstrumentFactory::Envelope()
{
	double output;
	double m_ramp;
	double m_time = 0.0;
	double m_duration = m_wave.size() / 44100.;

	for (int i = 0; i < m_wave.size(); i++, m_time += 1 / 44100.)
	{
		if (m_time < m_attack)
		{
			m_ramp = (m_time / m_attack);
		}
		if (m_time >(m_duration - m_release))
		{
			m_ramp = (m_duration - m_time) / m_release;
		}

		output = m_wave[i] * m_ramp;
		m_wave[i] = short(output);
	}
}