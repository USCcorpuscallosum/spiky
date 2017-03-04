#pragma once
#include "ofMain.h"

class Background
{
public:
	Background(MusicAnalysis* musicAnalysis);
	void changeBackground();
private:
	ofColor color;
	ofImage backgroundImage;
	MusicAnalysis* musicAnalysis;
    Terrain* terrain;
};
