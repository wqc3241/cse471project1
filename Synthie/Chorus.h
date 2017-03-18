/**
 * \file Chorus.h
 *
 * \author Xingchen Xiao
 *
 * 
 */


#pragma once
#include "Effect.h"
class Chorus :public CEffect
{
public:
	Chorus();
	virtual ~Chorus();

	virtual void Process(double *input, double *output) override;

	virtual void Start() override;

	virtual bool Generate() override;
};

