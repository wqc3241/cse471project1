#include "stdafx.h"
#include "Flange.h"


Flange::Flange()
{

}



Flange::~Flange()
{
}



void Flange::Start()
{
	mWrloc = 0;
	mRdloc = 0;
	mOutL.resize(MAXQUEUESIZE);
	mOutR.resize(MAXQUEUESIZE);
}

bool Flange::Generate()
{
	return true;
}

void Flange::Process(double* input, double* output)
{
	double delayVariance = (RANGE * mDelay) * sin(2 * PI * RATE);
	double newDelay = mDelay + delayVariance;

	mWrloc = (mWrloc + 1) % MAXQUEUESIZE;
	m_queueL[mWrloc] = input[0];

	int delayLength = int((newDelay * m_sampleRate + 0.5)) * 2;
	int rdloc = (mWrloc + MAXQUEUESIZE - delayLength) % MAXQUEUESIZE;

	// Wet
	output[0] = input[0] / 3 + m_queueL[rdloc] / 3 + (mOutL[rdloc] * LEVEL) / 3;
	output[0] *= mWet;

	// Dry
	output[0] += input[0] * mDry;
	mOutL[mWrloc] = output[0];


	mWrloc = (mWrloc + 1) % MAXQUEUESIZE;
	m_queueR[mWrloc] = input[1];
	rdloc = (mWrloc + MAXQUEUESIZE - delayLength) % MAXQUEUESIZE;

	// Wet
	output[1] = input[1] / 3 + m_queueR[(rdloc + 1) % MAXQUEUESIZE] / 3 + (mOutR[(rdloc + 1) % MAXQUEUESIZE] * LEVEL) / 3;
	output[1] *= mWet;

	// Dry
	output[1] += input[1] * mDry;

	mOutR[mWrloc] = output[1];
}
