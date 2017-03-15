#pragma once
#include "Instrument.h"
#include "AdditiveSines.h"
#include "Amp.h"

#include <list>

using namespace std;

class CAdditiveInstrument :
	public CInstrument
{
public:
	CAdditiveInstrument();
	virtual ~CAdditiveInstrument();
	CAdditiveInstrument(double bpm);

	virtual void Start() override;

	virtual bool Generate() override;
	virtual void SetNote(CNote *note) override;

	void SetNextNote(CNote* next_note);

	void AddHarmonics(std::wstring harmonics);

	void SetFreq(double freq) { m_sinewave.SetFreq(freq); }
	void SetAmplitude(double amp) { m_sinewave.SetAmp(amp); }
	void SetDuration(double duration) { m_duration = duration; }

private:

	double m_duration;
	double m_time;
	double m_crossFade = 0;

	CAdditiveSines m_sinewave;

	// the next additive instrument - for cross fade!
	CAdditiveInstrument* m_next = nullptr;
	CAmp m_amp;
	CADSR* m_ADSR;
};
