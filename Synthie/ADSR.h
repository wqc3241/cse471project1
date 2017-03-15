#pragma once
#include "AudioNode.h"

class CADSR:
	public CAudioNode
{
public:
	CADSR();
	virtual ~CADSR();

	//! Start the node generation
	virtual void Start() override;
	//! Cause one sample to be generated
	virtual bool Generate()  override;

	// GETTERS
	double GetEnvelopeLevel() { return m_level; }

	// SETTERS
	void SetDuration(double duration) { m_duration = duration; }

	// SETTERS

	// set attack length
	void SetAttack(double attack) { m_attack = attack; }
	// set decay length
	void SetDecay(double decay) { m_decay = decay; }
	// set the sustain level
	void SetSustain(double sustain) { m_sustain = sustain; }
	// set release length
	void SetRelease(double release) { m_release = release; }

protected:
	// how long the sample audio goes
	double m_duration;
	// keep track of time
	double m_time;
	// the envelope level
	double m_level;

	double m_attack;
	double m_decay;
	double m_sustain; // sustain level
	double m_release;
};

