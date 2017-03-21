#pragma once
#include "AudioNode.h"

class CCAR :
	public CAudioNode
{
public:
	CCAR();
	virtual ~CCAR();

private:
	CAudioNode* m_source;
	double m_attack;
	double m_release;
	double m_duration;
	double m_time;

public:
	virtual void Start();
	virtual bool Generate();
	void SetSource(CAudioNode* const &source) { m_source = source; }
	void SetDuration(double duration) { m_duration = duration; }
};

