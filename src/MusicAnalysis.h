#pragma once

#include "ofMain.h"

class MusicAnalysis {
public:
	const int SAMPLING_BANDS = 512;

	MusicAnalysis();
	void setSound(string);
	float getVolume();
	float getPitch();
	bool getBeat();
	float getVolumeOfRange(float min, float max);

	
private:
	ofSoundPlayer soundPlayer;
	float sampleRate;
};
