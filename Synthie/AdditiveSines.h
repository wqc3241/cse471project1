#pragma once
#include "AudioNode.h"
#include <vector>

using namespace std;

class CAdditiveSines :
	public CAudioNode
{
public:
	CAdditiveSines();
	virtual ~CAdditiveSines();

	// used for implementing the Vibrato effect
	struct Vibrato
	{
		double vibrato = 0;
		double vibrato_rate = 0;
	};

	//! Start the node generation
	virtual void Start() override;
	//! Cause one sample to be generated
	virtual bool Generate() override;
	// set the wave tables for harmonics and vibrato 
	void SetWavetables();

	// add a harmonic to the vector of harmonics
	void AddHarmonic(double harmonic) { m_harmonics.push_back(harmonic); }
	// adjust sample to match number of harmonics
	void ImplementHarmonics(double* sample, double time);
	// adjust sample to implement vibrato sound
	void ImplementVibrato(double* sine_rads, double* vibrato_rads);
	// generates a cross fade effect on a sample 
	void GenerateCrossfade(double time, double crossfade_dur);

	// GETTERS
	vector<double> GetHarmonics() { return m_harmonics; }
	bool GetVibratoFlag() { return m_implement_vibrato; }
	bool GetCrossfadeFlag() { return m_implement_crossfade; }
	double GetCrossfadeStartTime() { return m_crossfade_start_time; }

	// SETTERS

	// cross fade stuff
	void SetCrossfadeFlag(bool flag) { m_implement_crossfade = flag; }
	void SetCrossfadeStartTime(double time) { m_crossfade_start_time = time; }
	// vibrato stuff
	void SetVibratoFlag(bool flag) { m_implement_vibrato = flag; }
	void SetVibrato(double vibrato) { m_vibrato_effect.vibrato = vibrato; }
	void SetVibratoRate(double vibrato_rate) { m_vibrato_effect.vibrato_rate = vibrato_rate; }
	// wave stuff
	void SetNextWave(CAdditiveSines* next) { m_next_wave = next; }
	// GETTERS

	// get the size of the wave table
	double GetWavetableSize() { return m_wavetable.size(); }
	// get the frequency
	double GetFreq() { return m_freq; }
	// get the amplitude
	double GetAmp() { return m_amp; }
	// get the phase
	double GetPhase() { return m_phase; }
	// get the filter (flag)
	bool GetFilter() { return m_filter; }

	// SETTERS

	// sets the frequency
	void SetFreq(double f) { m_freq = f; }
	// sets the amplitude
	void SetAmp(double a) { m_amp = a; }

protected:
	std::vector<double> m_wavetable;  // wave table
	double m_freq; // frequency
	double m_amp;  // amplitude
	int m_phase;  // phase
	bool m_filter;  // filter?

private:
	// list of harmonics
	vector<double> m_harmonics;
	// keep track of time
	double m_time;
	// vibrato?
	bool m_implement_vibrato = false;
	// cross fade?
	bool m_implement_crossfade = false;
	// when to begin the cross fade
	double m_crossfade_start_time;
	// the vibrato effect struct object
	Vibrato m_vibrato_effect;
	// the wave next to the current wave
	CAdditiveSines* m_next_wave = nullptr;
};

