#pragma once
#include "ofMain.h"

class Background
{
public:
	Background(MusicAnalysis*);
	void changeBackground();
private:
	ofColor color;
	ofImage backgroundImage;
	MusicAnalysis* musicAnalysis;
};