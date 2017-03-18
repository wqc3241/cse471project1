/**
 * \file Chorus.cpp
 *
 * \author Xingchen Xiao
 */




#include "stdafx.h"
#include "Chorus.h"


Chorus::Chorus()
{
}


Chorus::~Chorus()
{
}

void Chorus::Start()
{
	mWrloc = 0;
	mRdloc = 0;
	m_queueL.resize(MAXQUEUESIZE);
	m_queueR.resize(MAXQUEUESIZE);
}

bool Chorus::Generate()
{
	return true;
}


void Chorus::Process(double* input, double* output)
{

	double delayVariance = (RANGE * mDelay) * sin(2 * PI * RATE);
	double newDelay = mDelay + delayVariance;

	mWrloc = (mWrloc + 1) % MAXQUEUESIZE;
	m_queueL[mWrloc] = input[0];
	mWrloc = (mWrloc + 1) % MAXQUEUESIZE;
	m_queueR[mWrloc] = input[1];

	int delayLength = int((newDelay * GetSampleRate() + 0.5)) * 2;
	int rdloc = (mWrloc + MAXQUEUESIZE - delayLength) % MAXQUEUESIZE;

	output[0] = input[0] / 2 + m_queueL[rdloc] / 2;
	output[0] *= mWet;
	output[0] += input[0] * mDry;

	mWrloc = (mWrloc + 1) % MAXQUEUESIZE;
	m_queueR[mWrloc] = input[1];
	rdloc = (mWrloc + MAXQUEUESIZE - delayLength) % MAXQUEUESIZE;
	output[1] = input[1] / 2 + m_queueR[rdloc] / 2;
	output[1] *= mWet;


	output[1] += input[1] * mDry;

}