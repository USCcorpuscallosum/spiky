#include "Galaxy.h"
#include "Orbital.h"
#include "MusicAnalysis.h"

Galaxy::~Galaxy()
{
	for (size_t i = 0; i < orbitals.size(); i++)
	{
		delete orbitals[i];
	}
}

void Galaxy::initialize(const OrbitalDef& def, MusicAnalysis* analysis)
{
	mAnalysis = analysis;
	createOrbitalFromDef(def, 0, nullptr);
}

void Galaxy::initialize(int maxLevel, float radiusScalar, float rotationScalar, float speedScalar, MusicAnalysis* analysis)
{
	mAnalysis = analysis;

	for (int i = 0; i < numOfOrbitals; i++)
	{
		Orbital* orb = new Orbital(0, maxLevel, radiusScalar, rotationScalar + 2, speedScalar + 2, analysis);

		float x = ofRandom(-spawnRange / 2, spawnRange / 2);
		float y = ofRandom(2, spawnRange / 2);
		float z = ofRandom(-spawnRange / 2, spawnRange / 2);
		ofVec3f myPosition(x, y, z);

		ofVec3f myPos = myPosition;
		orb->setPosition(myPos);
		orbitals.push_back(orb);
	}
}

void Galaxy::update()
{
	for (size_t i = 0; i < orbitals.size(); i++)
	{
		orbitals[i]->update();
	}
}

void Galaxy::draw() const
{
	for (size_t i = 0; i < orbitals.size(); i++)
	{
		orbitals[i]->draw();
	}
}

void Galaxy::debugReload()
{
	for (size_t i = 0; i < orbitals.size(); i++)
	{
		orbitals[i]->debugReload();
	}
}

Orbital* Galaxy::createOrbitalFromDef(const OrbitalDef& def, int level, Orbital* parent)
{
	Orbital* sun = new Orbital(def, level, mAnalysis);
	if (parent) sun->setParent(*parent);
	orbitals.push_back(sun);

	for (size_t i = 0; i < def.children.size(); i++)
	{
		createOrbitalFromDef(def.children[i], level + 1, sun);
	}

	return sun;
}
