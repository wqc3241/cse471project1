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

	virtual void Start() override;
	virtual bool Generate() override;
	// set the wave tables for harmonics and vibrato 
	void SetWavetables();

	// add a harmonic to the vector of harmonics
	void AddHarmonic(double harmonic) { m_harmonics.push_back(harmonic); }

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
	void SetVibrato(double vibrato) { m_vibrato = vibrato; }
	void SetVibratoRate(double vibrato_rate) { m_vibratoRate = vibrato_rate; }
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

	vector<double> m_harmonics;
	double m_time;
	bool m_implement_vibrato = false;
	bool m_implement_crossfade = false;

	double m_crossfade_start_time;

	double m_vibrato;
	double m_vibratoRate;
	CAdditiveSines* m_next_wave = nullptr;
};

