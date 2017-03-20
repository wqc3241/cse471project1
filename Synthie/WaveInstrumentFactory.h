#pragma once
#include "WaveInstrument.h"
#include "audio/DirSoundSource.h"
#include <vector>

class CWaveInstrumentFactory
{
public:
	CWaveInstrumentFactory();
	~CWaveInstrumentFactory();

public:
	CWaveInstrument *CreateInstrument();
	void SetNote(CNote *note);
	bool LoadFile(const char *filename);
	void ChangePitch(double rate);
	void CrossFade(const char *filename);
	void ChangeDuration();
	void CWaveInstrumentFactory::Envelope();

	void SetFade(double fade) { m_fade = fade; }
	void SetAttack(double attack) { m_attack = attack; }
	void SetRelease(double release) { m_release = release; }
	void SetDuration(double duration) { m_duration = duration; }

private:
	std::vector<short> m_wave;
	double m_fade;
	double m_attack;
	double m_release;
	double m_duration;
};

