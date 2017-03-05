#pragma once

#include "ofMain.h"
#include "MusicAnalysis.h"
#include "Terrain.h"

class Terrain;
class MusicAnalysis;

/*
  Control backgroud color based on information from the music and the terrain
*/
class Background
{
public:
	Background();
	void draw();
	void setTerrain(Terrain* t) { terrain = t; }

private:
	ofImage backgroundImage;
	MusicAnalysis* musicAnalysis;
	Terrain* terrain;

	ofPlanePrimitive plane;
	ofShader shader;

};
