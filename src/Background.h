#pragma once

#include "ofMain.h"
#include "MusicAnalysis.h"
#include "Terrain.h"

class Terrain;
class MusicAnalysis;

class Background
{
public:
	Background(MusicAnalysis*);
	void changeBackground();

private:
	ofColor color;
	ofImage backgroundImage;
	MusicAnalysis* musicAnalysis;
	Terrain* terrain;

};
