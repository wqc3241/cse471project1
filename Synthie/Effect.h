/**
 * \file Effect.h
 *
 * \author Xingchen Xiao
 *
 * 
 */



#pragma once
#include "Instrument.h"
#include <vector>
class CEffect : public CInstrument
{
public:
	virtual ~CEffect();

	virtual void Process(double *frameIn, double *frameOut) = 0;

	void SetDelay(double delay) { mDelay = delay; }

	void SetWet(double wet) { mWet = wet; }

	void SetDry(double dry) { mDry = dry; }

	void SetThreshold(double threshold) { mThreshold = threshold; }

	virtual void SetNote(CNote *note) override;

protected:
	CEffect();

	// Controllable effects sends
	double	mDelay;
	double	mWet;
	double	mDry;
	double  mThreshold;

	int mWrloc;
	int mRdloc;

	double mTime = 5.0;

	std::vector<double> m_queueL;
	std::vector<double> m_queueR;

	const int MAXQUEUESIZE = 200000;
	const double M_PI = 3.14159265359;
	const double RANGE = .5;
	const int RATE = 2;
	const int LEVEL = 2;
};

