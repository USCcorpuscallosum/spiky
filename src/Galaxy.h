#pragma once
#include "Orbital.h"
#include "MusicAnalysis.h"

class Galaxy : public ofNode
{
public:
	Galaxy();
	void initialize(int ml, float rs, float rotS, float ss, MusicAnalysis* ma);
	void mainUpdate();
private:
	int numOfOrbitals = 2;
	float spawnRange = 80;
	vector<Orbital*> orbitals;
	MusicAnalysis* analysis;
};