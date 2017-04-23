#pragma once
#include "Orbital.h"

class Galaxy : public ofNode
{
public:
	Galaxy(int ml, float rs, float rotS, float ss);
	void mainUpdate();
private:
	int numOfOrbitals = 5;
	float spawnRange = 100;
	vector<Orbital*> orbitals;
};