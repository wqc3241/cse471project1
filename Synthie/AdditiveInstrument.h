#pragma once
#include "Instrument.h"
#include "AdditiveSines.h"

class CAdditiveInstrument :
	public CInstrument
{
public:
	CAdditiveInstrument();
	virtual ~CAdditiveInstrument();
public:
	virtual void Start();
	virtual bool Generate();

	void SetFreq(double f) { m_sinewave.SetFreq(f); }
	void SetAmplitude(double& a) { m_sinewave.SetAmplitude(a); }
	void SetDuration(double d) { m_duration = d * GetSecondsPerBeat(); }
	void SetCrossFadeIn(double in) { m_attack = in * GetSecondsPerBeat(); }
	void SetCrossFadeOut(double out) { m_release = out * GetSecondsPerBeat(); }
	virtual void SetNote(CNote *note);

private:
	CAdditiveSines   m_sinewave;
	double m_duration;
	double m_time;
	double m_release;
	double m_attack;
	double m_crossFadeIn;
	double m_crossFadeOut;
	double m_sustain;
	double m_decay;
};
