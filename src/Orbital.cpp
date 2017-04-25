#include "Orbital.h"
#include <cmath>
#include <iostream>

Orbital::Orbital(Orbital* parent, int level, int maxLevel, float radiusScalar, float rotationScalar, float speedScalar, MusicAnalysis* analysis)
{
	setMusicAnalysis(analysis);

	// Setup ring
	ring.setMusicAnalysis(analysis);
	ring.setAmplitude(2);
	auto& ringColor = ring.getColorCycler();
	ringColor.mStartHue = 0.0;
	ringColor.mEndHue = 1.0;
	ringColor.mRepeat = ColorCycler::PingPong;
	ringColor.mDuration = 3.0;
	
	ring.setInnerRadius(getRadius() + .1);
	ring.setOuterRadius(ring.getInnerRadius() + ((float)1 / level) * 5);

	this->parent = parent;
	this->level = level;
	this->maxLevel = maxLevel;

	radiusScale = radiusScalar;
	rotationScale = rotationScalar;
	speedScale = speedScalar;

	angle = ofRandom(0, 360);
	numOfChildren = floor(ofRandom(2, 2));

	float levelMultiplier = pow(maxLevel + 1 - level, 2);
	setRadius(levelMultiplier * radiusScale * ofRandom(.8f, 1.2f) * .5);
	rotRadius = levelMultiplier * rotationScale * ofRandom(.8f, 1.2f);
	speed = (1 / levelMultiplier) * speedScale * ofRandom(.8f, 1.2f);
	
	if (level < maxLevel) createOrbitals();
}

void Orbital::mainUpdate()
{
	setOrbitalPos();
	update();
	draw();

	ring.setOrientation(ofVec3f(0, ofGetElapsedTimef() * 25.0, 10.0));
	ring.update();

	ofPushMatrix();
	ofTranslate(getPosition());
	ring.customDraw();
	ofPopMatrix();

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
