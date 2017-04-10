#pragma once

#include "ofMain.h"
#include "MusicAnalysis.h"

class Terrain
{
public:
	Terrain();
	Terrain(float init_length, float init_width);
	Terrain(float init_length, float init_width, float init_skip);
	void setMusicAnalysis(MusicAnalysis* analysis) { musicAnalysis = analysis; }
	void setActiveShader(int index) { activeShader = index; }
	virtual void initializeTerrain();	// Add vertices to mesh
	void changeAllColors();
	void changeHeight();
	void draw();
	void drawWithoutShader();
	ofColor getColor() const { return color; }
	virtual void calculateNewPosForIndex(int, float);

	const float CYCLE_SPEED = 25;

protected:
	ofMesh mesh;
	ofMesh baseMesh;
	MusicAnalysis* musicAnalysis;
	ofColor color;
	float hue = -1, saturation, brightness;

	vector<string> shaderNames;
	vector<ofShader> shaders;
	int activeShader;
	float length;
	float width;
	float skip;
	float heightSpeed;
	float heightAcceleration;

	float randomPosScalar;
};
