/**
 * \file Compression.h
 *
 * \author Xingchen Xiao
 *
 * 
 */





#pragma once

#include "Effect.h"

class Compression : public CEffect
{
public:
	Compression();
	virtual ~Compression();

	virtual void Process(double *input, double *output) override;

	virtual void Start() override;

	virtual bool Generate() override;
};

