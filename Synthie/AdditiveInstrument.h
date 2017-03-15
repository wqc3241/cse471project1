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

	//! Start the node generation
	virtual void Start() override;
	//! Cause one sample to be generated
	virtual bool Generate() override;
	virtual void SetNote(CNote *note) override;

	// used for cross fading, allow notes to play over each other
	void SetNextNote(CNote* next_note);
	// add harmonics to the additive wave
	void AddHarmonics(std::wstring harmonics);

	// SETTERS
	void SetFreq(double freq) { m_sinewave.SetFreq(freq); }
	void SetAmplitude(double amp) { m_sinewave.SetAmp(amp); }
	void SetDuration(double duration) { m_duration = duration; }

private:
	// how long an audio sample goes
	double m_duration;
	// keep track of time
	double m_time;
	// the duration of the cross fade
	double m_crossfade_duration = 0;
	// the additive wave object
	CAdditiveSines m_sinewave;
	// the next additive instrument - for cross fade!
	CAdditiveInstrument* m_next = nullptr;

	CAmp m_amp_filter;

	CADSR* m_envelope;
};
