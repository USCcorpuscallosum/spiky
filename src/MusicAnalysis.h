#pragma once

#include "ofMain.h"

class MusicAnalysis {
public:
	MusicAnalysis();
	void togglePlay();	// Pause the song
	void changeSong(int);
	void loadSongs(vector<string>);	// Load songs(songs' filenames) into soundPlayer

	float getVolume();	// Returns current volume
	float getPitch();	// Returns current pitch (Hardcoded now)
	bool getBeat();	// Returns current beat (Hardcoded now)
	float getVolumeOfRange(float min, float max, float* outMaxVolume = nullptr);	// Sum the volume of sound in a certain range of frequency

	const int SAMPLING_BANDS = 512; 
	
private:
	vector<ofSoundPlayer*> soundPlayer;	// Store songs
	float sampleRate;
	ofSoundPlayer* currentSong;
	int numOfSongs;
	bool isPlaying;
};
