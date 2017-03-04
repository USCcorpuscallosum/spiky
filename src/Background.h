#pragma once
#include "ofMain.h"

class Background
{
public:
	Background(musicAnalysis*);
	void changeBackground();
private:
	ofColor color;
	ofImage backgroundImage;
	MusicAnalysis* musicAnalysis;
};