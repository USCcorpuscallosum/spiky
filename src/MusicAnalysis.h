#pragma once
#ifndef MUSICANALYSIS_H
#define MUSICANALYSIS_H
#include "ofMain.h"

class MusicAnalysis{
public:
    MusicAnalysis(string filename);
    float getVolume();
    float getPitch();
    float getBeat();
    float getVolumeOfRange(float min, float max);
    
private:
    ofSoundPlayer sound_player;
};

#endif
