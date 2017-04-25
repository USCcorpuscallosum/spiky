#pragma once
#include "Orbital.h"
#include "MusicAnalysis.h"

class Galaxy : public ofNode
{
public:
	struct OrbitalDef {
		/** Orbit distance from the parent orbital */
		float orbitRadius;

		// Planet properties
		float radius;
		float amplitude;
		ofColor color;

		/** Planets which orbit around us */
		std::vector<OrbitalDef> children;
	};

	Galaxy();
	void initialize(int maxLevel, float radiusScalar, float rotationScalar, float speedScalar, MusicAnalysis* analysis);
	void mainUpdate();
private:
	int numOfOrbitals = 2;
	float spawnRange = 80;
	vector<Orbital*> orbitals;
	MusicAnalysis* analysis;
};
