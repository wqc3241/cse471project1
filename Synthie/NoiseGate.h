/**
 * \file NoiseGate.h
 *
 * \author Xingchen Xiao
 *
 * 
 */


#pragma once
#include "Effect.h"
#include "AR.h"
class NoiseGate : public CEffect
{
public:
	NoiseGate();
	virtual ~NoiseGate();


	virtual void Process(double *input, double *output) override;

	virtual void Start() override;

	virtual bool Generate() override;

private:
	double mGateL;
	double mGateR;
};

