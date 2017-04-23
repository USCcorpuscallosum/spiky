#include "Orbital.h"
#include <cmath>
#include <iostream>

Orbital::Orbital(Orbital* p, int l, int ml, float rs, float rotS, float ss, MusicAnalysis* ma)
{
	std::cout << "In Orbital Constructor" << std::endl;

	setMusicAnalysis(ma);

	// Setup ring
	ring.setMusicAnalysis(ma);
	ring.setInnerRadius(12);
	ring.setOuterRadius(25);
	ring.setAmplitude(4);
	auto& ringColor = ring.getColorCycler();
	ringColor.mStartHue = 0.0;
	ringColor.mEndHue = 1.0;
	ringColor.mRepeat = ColorCycler::PingPong;
	ringColor.mDuration = 3.0;


	parent = p;
	level = l;
	maxLevel = ml;

	radiusScale = rs;
	rotationScale = rotS;
	speedScale = ss;

	angle = ofRandom(0, 360);
	numOfChildren = floor(ofRandom(1, 1));

	float levelMultiplier = pow(maxLevel + 1 - level, 2);
	setRadius(levelMultiplier * radiusScale * ofRandom(.8f, 1.2f) * .5);
	rotRadius = levelMultiplier * rotationScale * ofRandom(.8f, 1.2f);
	speed = (1 / levelMultiplier) * speedScale * ofRandom(.8f, 1.2f);
	
	if (level < maxLevel) createOrbitals();
	std::cout << "Ending Orbital Constructor" << std::endl;
}

void Orbital::mainUpdate()
{
	setOrbitalPos();
	update();
	draw();

	ring.setOrientation(ofVec3f(0, ofGetElapsedTimef() * 25.0, 10.0));
	ring.update();

	std::cout << "MY Position" << getPosition() << std::endl;

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
		angle += speed;

		float x = sin(ofDegToRad(angle));
		float y = cos(ofDegToRad(angle));
		ofVec3f turnVec = ofVec3f(x, 0, y);

		std::cout << "SIN" << sin(ofDegToRad(angle)) << std::endl;
		ofVec3f offset = rotRadius * turnVec;
		//ofVec3f offset(50, 50, 0);
		ofVec3f newPos = parentPos + offset;
		setPosition(newPos);
	}
}

void Orbital::createOrbitals()
{
	for (int i = 0; i < numOfChildren; i++)
	{
		Orbital* orb = new Orbital(this, level+1, maxLevel, radiusScale, rotationScale, speedScale, analysis);
		children.push_back(orb);
		orb->setMusicAnalysis(analysis);
	}
}