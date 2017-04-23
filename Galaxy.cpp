#include "Galaxy.h"

Galaxy::Galaxy(int ml, float rs, float rotS, float ss)
{
	for (int i = 0; i < numOfOrbitals; i++)
	{
		Orbital* orb = new Orbital(NULL, 0, ml, rs, rotS, ss);

		float x = ofRandom(-spawnRange / 2, spawnRange / 2);
		float y = ofRandom(-spawnRange / 2, spawnRange / 2);
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