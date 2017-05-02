#pragma once

#include "ofMain.h"
#include <map>
#include <ofxAudioAnalyzer.h>
#include "ofFmodSoundPlayerExtended.h"

class MusicAnalysis {
public:
	enum RangeType {
		//RANGE_SUBBASS,
		RANGE_BASS,
		RANGE_MID,
		RANGE_HIGHMID,
		RANGE_HIGH,

		RANGE_MAX
	};

	struct Range {
		float minFreq, maxFreq;
		float volume;
		float maxVolume;

		Range()
			: minFreq(0)
			, maxFreq(0)
			, volume(0)
			, maxVolume(0) {
		}

		Range(float min_, float max_)
			: minFreq(min_)
			, maxFreq(max_)
			, volume(0)
			, maxVolume(0) {
		}
	};

	MusicAnalysis();
	~MusicAnalysis();

	/** Switch to the specified player and start playing. */
	void setPlayer(ofFmodSoundPlayerExtended* player);
	void play();
	void pause();
	bool isPaused() const;

	void update();
	Range& getRange(RangeType type) { return ranges[type]; }
	map<RangeType, Range>& getRanges() { return ranges; }

	/** Get the frequency spectrum bins. Remapped to 0..1. */
	const vector<float>& getSpectrum() const { return spectrum; }
	/** Get harmonic pitch class profiles. Roughly represents tonality. */
	const vector<float>& getHPCP() const { return hpcp; }
	/** Get the general loudness (RMS power). */
	float getVolume() const { return rms; }
	/** Get the power means (another form of loudness). */
	float getPower() const { return power; }
	/** Get the primary pitch frequency. */
	float getPitch() const { return pitchFreq; }
	/** Get the High Frequency Content. This is good for detecting beat onsets. */
	float getHFC() const { return hfc; }
	/** Get decay (sound dropoff). This is good for finding when sounds attack and fade out. */
	float getDecay() const { return strongDecay; }
	/** Get normalized (0..1) decay. */
	float getDecayNormalized() const { return strongDecayNorm; }
	/** Get whether there is currently a beat/onset. */
	bool isOnBeat() const { return onBeat; }

	ofxAudioAnalyzer& getAnalyzer() { return analyzer; }

	const int SAMPLE_RATE = 44100;
	const int BUFFER_SIZE = 1024; // 512 bins

private:
	void getRangeVolume(Range &range, vector<float> &spectrum) const;

	ofFmodSoundPlayerExtended* currentPlayer = nullptr;
	ofxAudioAnalyzer analyzer;
	float smoothing = 0.4;

	map<RangeType, Range> ranges;
	vector<float> spectrum;
	vector<float> hpcp;
	float rms = 0, power = 0, pitchFreq = 0, hfc = 0, strongDecay = 0;
	float strongDecayNorm = 0;
	bool onBeat = false;
	
};
