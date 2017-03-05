#pragma once

#include "ofMain.h"
#include "MusicAnalysis.h"
#include "Terrain.h"

class Terrain;
class MusicAnalysis;

class Background
{
public:
	Background();
	void changeBackground();
	void setTerrain(Terrain* t) { terrain = t; }

private:
	ofColor color;
	ofImage backgroundImage;
	MusicAnalysis* musicAnalysis;
	Terrain* terrain;

};
