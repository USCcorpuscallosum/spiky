#pragma once
#include <ofNode.h>

class Galaxy : public ofNode
{
public:
	struct OrbitalDef {
		/** Orbit distance from the parent orbital */
		float orbitRadius;
		/** Time to complete an orbit in sec */
		float orbitInterval;
		float orbitAngle;

		// Planet properties
		float radius;
		float amplitude;
		ofFloatColor color;
		float seed;

		/** Planets which orbit around us */
		std::vector<OrbitalDef> children;
	};

	~Galaxy();

	void initialize(const OrbitalDef& def, class MusicAnalysis* analysis);
	void initialize(int maxLevel, float radiusScalar, float rotationScalar, float speedScalar, class MusicAnalysis* analysis);

	void update();
	void draw() const override;

	void debugReload();

private:
	class Orbital* createOrbitalFromDef(const OrbitalDef& def, int level, Orbital* parent);

	int numOfOrbitals = 2;
	float spawnRange = 80;
	vector<class Orbital*> orbitals;
	class MusicAnalysis* mAnalysis = nullptr;
	
};
