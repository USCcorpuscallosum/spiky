#pragma once
#include "Globe.h"
#include "Galaxy.h"
#include "PlanetRing.h"
#include <vector>

class Orbital : public Globe
{
public:
	Orbital(const Galaxy::OrbitalDef& def, int level, MusicAnalysis* analysis);
	Orbital(int level, int maxLevel, float radiusScalar, float rotationScalar, float speedScalar, MusicAnalysis* analysis);
	void update() override;
	void draw() const override;
	void customDraw() override;

	int getLevel() { return level; }

private:
	void setOrbitalPos();
	void createOrbitals();

	int level;
	int maxLevel = 0;

	float radiusScale = 1;
	float rotationScale = 1;
	float speedScale = 1;

	float rotRadius = 0;
	float angle;
	float speed;
	float orbitAngle = 0;

	PlanetRing ring;

	int numOfChildren;
	vector<Orbital*> children;
};
