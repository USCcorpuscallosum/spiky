#pragma once
#include "Globe.h"
#include "PlanetRing.h"
#include <vector>

class Orbital : public Globe
{
public:
	Orbital(Orbital* parent, int level, int maxLevel, float radiusScalar, float rotationScalar, float speedScalar, MusicAnalysis* analysis);
	void mainUpdate();
	void setOrbitalPos();
	void createOrbitals();

private:
	Orbital* parent;
	int level;
	int maxLevel;

	float radiusScale; 
	float rotationScale;
	float speedScale;

	float rotRadius;
	float angle;
	float speed;

	PlanetRing ring;

	int numOfChildren;
	vector<Orbital*> children;
};
