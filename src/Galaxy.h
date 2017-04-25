#pragma once
#include "Orbital.h"
#include "MusicAnalysis.h"

class Galaxy : public ofNode
{
public:
	Galaxy();
	void initialize(int maxLevel, float radiusScalar, float rotationScalar, float speedScalar, MusicAnalysis* analysis);
	void mainUpdate();
private:
	int numOfOrbitals = 2;
	float spawnRange = 80;
	vector<Orbital*> orbitals;
	MusicAnalysis* analysis;
};
