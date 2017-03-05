#pragma once

#include "ofMain.h"

class MusicAnalysis {
public:
	MusicAnalysis(string filename);
	float getVolume();
	float* getPitch();
	float getBeat();
	float getVolumeOfRange(float min, float max);
	
private:
	ofSoundPlayer soundPlayer;
	int samplingBands;

};
