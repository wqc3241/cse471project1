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

	void SetHarmonics(int harmonic) { m_harmonics=harmonic; }

	void GenerateCrossfade(double time, double crossFadeDuration);

	int GetHarmonics() { return m_harmonics; }
	bool GetVibratoFlag() { return m_implement_vibrato; }
	bool GetCrossfadeFlag() { return m_implement_crossfade; }
	double GetCrossfadeStartTime() { return m_crossFadeStart; }

	void SetCrossfadeFlag(bool flag) { m_implement_crossfade = flag; }
	void SetCrossfadeStartTime(double time) { m_crossFadeStart = time; }

	void SetVibratoFlag(bool flag) { m_implement_vibrato = flag; }
	void SetVibrato(double vibrato) { m_vibrato = vibrato; }
	void SetVibratoRate(double vibrato_rate) { m_vibratoRate = vibrato_rate; }

	void SetNextWave(CAdditiveSines* next) { m_nextWave = next; }

	double GetWavetableSize() { return m_wavetable.size(); }
	double GetFreq() { return m_freq; }
	double GetAmp() { return m_amp; }
	double GetPhase() { return m_phase; }

	// sets the frequency
	void SetFreq(double f) { m_freq = f; }
	// sets the amplitude
	void SetAmp(double a) { m_amp = a; }

protected:
	std::vector<double> m_wavetable;  
	double m_freq; 
	double m_amp;  
	int m_phase; 

private:

	int m_harmonics;
	double m_time;
	bool m_implement_vibrato = false;
	bool m_implement_crossfade = false;

	double m_crossFadeStart;

	double m_vibrato;
	double m_vibratoRate;
	CAdditiveSines* m_nextWave = nullptr;
};

