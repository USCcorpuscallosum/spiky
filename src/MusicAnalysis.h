#pragma once

#include "ofMain.h"
#include <map>
#include <ofxAudioAnalyzer.h>
#include "ofFmodSoundPlayerExtended.h"

class MusicAnalysis {
public:
	enum RangeType {
		RANGE_SUBBASS,
		RANGE_BASS,
		RANGE_MID,
		RANGE_HIGHMID,
		RANGE_HIGH
	};

	struct Range {
		float minFreq, maxFreq;
		float volume;
		float maxVolume;

		Range()
		{
		}

		Range(float min_, float max_)
			: minFreq(min_)
			, maxFreq(max_)
			, volume(0)
			, maxVolume(0)
		{
		}
	};

	MusicAnalysis();
	~MusicAnalysis();
	void loadSongs(vector<string>);	// Load songs(songs' filenames) into soundPlayer
	void togglePlay();	// Pause the song
	void changeSong(int);

	void update();
	Range& getRange(RangeType type) { return ranges[type]; }
	map<RangeType, Range>& getRanges() { return ranges; }

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

private:
	void getRangeVolume(Range &range, vector<float> &spectrum);

	vector<ofFmodSoundPlayerExtended*> soundPlayers;
	ofFmodSoundPlayerExtended* currentSong;

	ofxAudioAnalyzer analyzer;
	int sampleRate, bufferSize;
	float smoothing = 0.0;

	map<RangeType, Range> ranges;
	vector<float> spectrum;
	float rms, power, pitchFreq, hfc, strongDecay;
	float strongDecayNorm;
	bool onBeat;
	
};
