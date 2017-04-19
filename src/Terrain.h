#pragma once

#include "ofMain.h"
#include "MusicAnalysis.h"

class Terrain : public ofNode
{
public:
	Terrain();

	void update();
	void customDraw() override;

	void setMusicAnalysis(MusicAnalysis* analysis) { musicAnalysis = analysis; }
	void setActiveShader(int index) { activeShader = index; }
	ofColor getColor() const { return color; }

	const float CYCLE_SPEED = 25;

private:
	void changeAllColors();
	void changeHeight();
	/** Add vertices to mesh */
	void initializeTerrain();
	void calculateNewPosForIndex(int index, float newHeight);

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
