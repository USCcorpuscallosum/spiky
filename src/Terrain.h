#pragma once

#include "ofMain.h"
#include "MusicAnalysis.h"

class Terrain
{
public:
	Terrain();
	void setSound(MusicAnalysis*);
	void initializeTerrain();
	void changeAllColors();
	void changeHeight();
	void draw();
    ofColor getColor();
private:
	ofMesh mesh;
	MusicAnalysis* musicAnalysis;
    ofColor color;

	float length;
	float width;
	float skip;
};
