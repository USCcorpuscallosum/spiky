#pragma once

#include "ofMain.h"
#include "MusicAnalysis.h"

class Terrain
{
public:
	Terrain(MusicAnalysis*);
	void initializeTerrain();
	void changeColor();
	void changeHeihgt();
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
