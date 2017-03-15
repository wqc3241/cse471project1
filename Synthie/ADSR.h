#pragma once
#include "AudioNode.h"

class CADSR:
	public CAudioNode
{
public:
	CADSR();
	virtual ~CADSR();

	virtual void Start() override;
	virtual bool Generate()  override;

	double GetEnvelopeLevel() { return m_level; }

	void SetDuration(double duration) { m_duration = duration; }
	void SetAttack(double attack) { m_attack = attack; }
	void SetDecay(double decay) { m_decay = decay; }
	void SetSustain(double sustain) { m_sustain = sustain; }
	void SetRelease(double release) { m_release = release; }

protected:
	double m_duration;
	double m_time;
	double m_level;

	double m_attack;
	double m_decay;
	double m_sustain;
	double m_release;

};

