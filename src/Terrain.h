#pragma once

#include "ofMain.h"
#include "ofCustomMaterial.h"
#include "ColorCycler.h"

class Terrain : public ofNode
{
public:
	Terrain();
	~Terrain();

	void update();
	void customDraw() override;

	void debugReload();
	void setLSD(bool on) { mode = on ? 1 : 0; };

	void setMusicAnalysis(class MusicAnalysis* analysis) { musicAnalysis = analysis; }

	const float CYCLE_SPEED = 25;
	const int ELEVATION_STEPS = 10;
	const int ELEVATION_MAX = 12;
	const int ELEVATION_MIN = -8;

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

	class Map* map = nullptr;

	ofCustomMaterial material;
	int mode = 0;

	float length;
	float width;
	float skip;
	float heightSpeed;
	float heightAcceleration;

	float randomPosScalar;
};
