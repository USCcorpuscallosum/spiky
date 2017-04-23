#include "Orbital.h"
#include <cmath>

Orbital::Orbital(Orbital* p, int l, int ml, float rs, float rotS, float ss)
{
	parent = p;
	level = l;
	maxLevel = ml;

	radiusScale = rs;
	rotationScale = rotS;
	speedScale = ss;

	numOfChildren = floor(ofRandom(2, 4));

	float levelMultiplier = pow(maxLevel + 1 - level, 2);
	setRadius(levelMultiplier * radiusScale * ofRandom(.8f, 1.2f));
	rotRadius = levelMultiplier * rotationScale * ofRandom(.8f, 1.2f);
	speed = (1 / levelMultiplier) * speedScale * ofRandom(.8f, 1.2f);
	
	if (level < maxLevel) createOrbitals();

}

void Orbital::mainUpdate()
{
	setOrbitalPos();
	update();

	for (int i = 0; i < children.size(); i++)
	{
		children[i]->mainUpdate();
	}
}

void Orbital::setOrbitalPos()
{
	
	if (parent != NULL)
	{
		ofVec3f parentPos = parent->getPosition();

		float x = sin(ofDegToRad(angle));
		float y = cos(ofDegToRad(angle));
		ofVec3f turnVec = ofVec3f(x, 0, y);

		ofVec3f offset = rotRadius * turnVec;
		ofVec3f newPos = parentPos + offset;
		setPosition(newPos);
	}
}

void Orbital::createOrbitals()
{
	for (int i = 0; i < numOfChildren; i++)
	{
		Orbital* orb = new Orbital(this, level+1, maxLevel, radiusScale, rotationScale, speedScale);
		children.push_back(orb);
	}
}