#pragma once

#include "ofMain.h"
#include "MusicAnalysis.h"

class Terrain
{
public:
	const float CYCLE_SPEED = 25;

	Terrain();
	Terrain(float init_length, float init_width);
	Terrain(float init_length, float init_width, float init_skip);
	void setMusicAnalysis(MusicAnalysis* analysis) { musicAnalysis = analysis; }
	void setActiveShader(int index) { activeShader = index; }
	void initializeTerrain();	// Add vertices to mesh
	void changeAllColors();
	void changeHeight();
	void draw();
	ofColor getColor() const { return color; }

private:
	ofMesh mesh;
	MusicAnalysis* musicAnalysis;
	ofColor color;
	float hue = -1, saturation, brightness;

	vector<string> shaderNames;
	vector<ofShader> shaders;
	int activeShader;
	float length;
	float width;
	float skip;
};
