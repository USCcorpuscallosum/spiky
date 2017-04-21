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

		Range() {
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
	void loadSongs(vector<string>);	// Load songs(songs' filenames) into soundPlayer
	void play();
	void pause();
	bool isPaused();
	/** Switch to the specified song and start playing. */
	void setSong(int index);
	/** Switch to the specified device and start playing. */
	void setDeviceId(int device);

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

private:
	void getRangeVolume(Range &range, vector<float> &spectrum);

	vector<ofFmodSoundPlayerExtended*> soundPlayers;
	ofFmodSoundPlayerExtended recordPlayer;
	ofFmodSoundPlayerExtended* currentSong;

	ofxAudioAnalyzer analyzer;
	int sampleRate, bufferSize;
	float smoothing = 0.0;

	map<RangeType, Range> ranges;
	vector<float> spectrum;
	vector<float> hpcp;
	float rms, power, pitchFreq, hfc, strongDecay;
	float strongDecayNorm;
	bool onBeat;
	
};
