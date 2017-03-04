#pragma once
#include "ofMain.h"
#ifndef MUSICANALYSIS_H
#define MUSICANALYSIS_H

class MusicAnalysis{
public:
    float getVolume();
    float getPitch();
    float getBeat();
    float getVolumeOfRange(float min, float max);
    
private:
    ofSoundPlayer sound_player;
};

#endif
