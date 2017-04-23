#include "Galaxy.h"
#include "MusicAnalysis.h"

Galaxy::Galaxy()
{
	std::cout << "Creating Galaxy" << std::endl;
}

void Galaxy::initialize(int ml, float rs, float rotS, float ss, MusicAnalysis* ma)
{
	std::cout << "Initializing Galaxy" << std::endl;

	for (int i = 0; i < numOfOrbitals; i++)
	{
		std::cout << "In Creation Loop" << std::endl;
		Orbital* orb = new Orbital(NULL, 0, ml, rs, rotS, ss, ma);
		std::cout << "Created Orbital" << std::endl;

		orb->setMusicAnalysis(ma);

		float x = ofRandom(-spawnRange / 2, spawnRange / 2);
		float y = ofRandom(-spawnRange / 2, spawnRange / 2);
		float z = ofRandom(-spawnRange / 2, spawnRange / 2);
		ofVec3f myPosition(x, y, z);

		ofVec3f myPos = myPosition;
		orb->setPosition(myPos);
		orbitals.push_back(orb);

		std::cout << myPos << std::endl;
	}
}

void Galaxy::mainUpdate()
{
	for (int i = 0; i < numOfOrbitals; i++)
	{
		orbitals[i]->mainUpdate();
	}
}