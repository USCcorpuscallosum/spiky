#pragma once

#include "ofMain.h"
#include "ofCustomMaterial.h"
#include "ColorCycler.h"

class Terrain : public ofNode
{
public:
	Terrain();

	void update();
	void customDraw() override;

	void debugReload();

	void setMusicAnalysis(class MusicAnalysis* analysis) { musicAnalysis = analysis; }

	const float CYCLE_SPEED = 25;

private:
	void setupMaterial();
	/** Add vertices to mesh */
	void initializeTerrain();
	void updateHeight();
	void calculateNewPosForIndex(int index, float newHeight);

	ofMesh mesh;
	ofMesh baseMesh;
	class MusicAnalysis* musicAnalysis;
	ColorCycler colorCycler;

	ofCustomMaterial material;

	float length;
	float width;
	float skip;
	float heightSpeed;
	float heightAcceleration;

	float randomPosScalar;
};
