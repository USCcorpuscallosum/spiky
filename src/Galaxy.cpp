#include "Galaxy.h"
#include "MusicAnalysis.h"

Galaxy::Galaxy()
{
}

void Galaxy::initialize(int maxLevel, float radiusScalar, float rotationScalar, float speedScalar, MusicAnalysis* analysis)
{
	for (int i = 0; i < numOfOrbitals; i++)
	{
		Orbital* orb = new Orbital(nullptr, 0, maxLevel, radiusScalar, rotationScalar + 2, speedScalar + 2, analysis);

		orb->setMusicAnalysis(analysis);

		float x = ofRandom(-spawnRange / 2, spawnRange / 2);
		float y = ofRandom(2, spawnRange / 2);
		float z = ofRandom(-spawnRange / 2, spawnRange / 2);
		ofVec3f myPosition(x, y, z);

		ofVec3f myPos = myPosition;
		orb->setPosition(myPos);
		orbitals.push_back(orb);
	}
}

void Galaxy::mainUpdate()
{
	for (int i = 0; i < numOfOrbitals; i++)
	{
		orbitals[i]->mainUpdate();
	}
}
