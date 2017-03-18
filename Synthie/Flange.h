/**
 * \file Flange.h
 *
 * \author Xingchen Xiao
 *
 * 
 */


#pragma once
#include "Effect.h"

class Flange :public CEffect
{
public:
	Flange();
	virtual ~Flange();

	virtual void Process(double *input, double *output) override;

	virtual void Start() override;

	virtual bool Generate() override;

private:
	std::vector<double> mOutL;
	std::vector<double> mOutR;
};

