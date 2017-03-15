#pragma once
#include "AudioNode.h"
#include "ADSR.h"

class CAmp :
	public CAudioNode
{
public:
	CAmp();
	virtual ~CAmp();

	//! Start the node generation
	virtual void Start();
	//! Cause one sample to be generated
	virtual bool Generate();

	// SETTERS
	void SetEnvelope(CADSR* envelope) { m_envelope = envelope; }
	void SetSource(CAudioNode* source) { m_source = source; }
	void SetDuration(double duration) { m_duration = duration; }

protected:
	// source of the audio node
	CAudioNode* m_source;
	// envelope object
	CADSR* m_envelope;
	// audio duration
	double m_duration;
	// keep track of time
	double m_time;
};

