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
	void SetEnvelope(CADSR* envelope) { m_ADSR = envelope; }
	void SetSource(CAudioNode* source) { m_source = source; }
	void SetDuration(double duration) { m_duration = duration; }

protected:
	CAudioNode* m_source;
	CADSR* m_ADSR;
	double m_duration;
	double m_time;
};

