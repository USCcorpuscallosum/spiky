#pragma once
#include "Globe.h"
#include "Galaxy.h"
#include <vector>

class Orbital : public Globe
{
public:
	Orbital(const Galaxy::OrbitalDef& def, int level, MusicAnalysis* analysis);
	Orbital(int level, int maxLevel, float radiusScalar, float rotationScalar, float speedScalar, MusicAnalysis* analysis);
	~Orbital();
	void update() override;
	void draw() const override;
	void customDraw() override;

	int getLevel() const { return level; }

private:
	void setOrbitalPos();
	void createOrbitals();

	int level = 0;
	int maxLevel = 0;

	float radiusScale = 1;
	float rotationScale = 1;
	float speedScale = 1;

	float rotRadius = 0;
	float angle = 0;
	float speed = 0;
	float orbitAngle = 0;

	class PlanetRing* ring = nullptr;

	int numOfChildren;
	vector<Orbital*> children;
};
