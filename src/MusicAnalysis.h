#pragma once

#include "ofMain.h"

class MusicAnalysis {
public:
	MusicAnalysis();
	void togglePlay();
	void addSong(ofSoundPlayer);
	void changeSong(int);
	void loadSongs(vector<string>);

	float getVolume();
	float getPitch();
	bool getBeat();
	float getVolumeOfRange(float min, float max, float* outMaxVolume = nullptr);

	const int SAMPLING_BANDS = 512;
	
private:
	vector<ofSoundPlayer> soundPlayer;
	float sampleRate;
	ofSoundPlayer* currentSong;
	int numOfSongs;
	bool isPlaying;
};
