#pragma once

#include "Terrain.h"
#include "ofMain.h"

class Globe : public Terrain
{
public:

	Globe();
	Globe(float init_length, float init_width, float radius, ofVec3f centerPoint);
	Globe(float init_length, float init_width, float radius, ofVec3f centerPoint, float init_skip);

	void initializeTerrain();
	void calculateNewPosForIndex(int, int, int, float);
private:
	float radius;
	ofVec3f centerPoint;
};