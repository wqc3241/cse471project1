#pragma once
#include "Instrument.h"
#include "AdditiveSines.h"
#include "AR.h"

class CAdditiveInstrument :
	public CInstrument
{
public:
	CAdditiveInstrument(void);
	virtual ~CAdditiveInstrument(void);
public:
	virtual void Start();
	virtual bool Generate();
	virtual void SetNote(CNote *note);

	void SetFreq(double f) { m_sinewave.SetFreq(f); }
	void SetAmplitude(double& a) { m_sinewave.SetAmplitude(a); }
	void SetDuration(double d) { m_duration = d * GetSecondsPerBeat(); }
	void SetCrossFadeIn(double i) { m_attack = i * GetSecondsPerBeat(); }
	void SetCrossFadeOut(double o) { m_release = o * GetSecondsPerBeat(); }

private:
	CAdditiveSines   m_sinewave;
	double m_duration;
	double m_time;
	double m_attack;
	double m_release;
	double m_sustain;
	double m_decay;
	double m_crossFadeIn;
	double m_crossFadeOut;
};

