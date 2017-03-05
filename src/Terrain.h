#pragma once

#include "ofMain.h"
#include "MusicAnalysis.h"

class Terrain
{
public:
	Terrain(); // maybe need a constructor with length and width parameters. KP
	Terrain(float init_length, float init_width);
	Terrain(float init_length, float init_width, float init_skip);
	void setMusicAnalysis(MusicAnalysis* analysis) { musicAnalysis = analysis; }
	void initializeTerrain();
	void changeAllColors();
	void changeHeight();
	void draw();
	ofColor getColor();
private:
	ofMesh mesh;
	ofShader shader;
	MusicAnalysis* musicAnalysis;
	ofColor color;

	float length;
	float width;
	float skip;
};
