#include "ofMain.h"
#include "Globe.h"
#include <cmath>

#define M_PI 3.14
#define M_PI_2 1.57

Globe::Globe() : Terrain() { radius = 100; }


Globe::Globe(float init_length, float init_width, float r, ofVec3f cp)
	: Terrain(init_length, init_width) { radius = r; centerPoint = cp; }

Globe::Globe(float init_length, float init_width, float r, ofVec3f cp, float init_skip)
		: Terrain(init_length, init_width, init_skip) {radius = r; centerPoint = cp;}



void Globe::initializeTerrain()
{
	int rings = 50;
	int sectors = 50;
	
	ofPrimitiveMode p;
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);

	float sphereCompleteness = 1; // 1.0 for whole sphere, 0.5 for hemisphere  
	float const R = 1. / (float)(rings - 1) * sphereCompleteness;
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	for (r = 0; r < rings; r++) 
	{
		for (s = 0; s < sectors; s++)
		{
			float const y = sin(-M_PI_2 + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);
			//mesh.addTexCoord(ofVec2f(s*S, r*R));
			mesh.addVertex(ofVec3f(x * radius, y * radius, z * radius));
			mesh.addNormal(ofVec3f(x, y, z));
		}
	}


	for (r = 0; r < rings; r++) 
	{
		for (s = 0; s < sectors; s++) 
		{
			mesh.addIndex(r * sectors + s);
			mesh.addIndex(r * sectors + (s + 1));
			mesh.addIndex((r + 1) * sectors + (s + 1));
			mesh.addIndex((r + 1) * sectors + s);
		}
	}
}

void Globe::calculateNewPosForIndex(int index, int x, int y, float newHeight)
{

}