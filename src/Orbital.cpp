#include "Orbital.h"
#include "Galaxy.h"
#include <cmath>

Orbital::Orbital(const Galaxy::OrbitalDef& def, int level, MusicAnalysis* analysis)
{
	this->level = level;
	setMusicAnalysis(analysis);

	rotRadius = def.orbitRadius;
	speed = 360.0 / def.orbitInterval;
	orbitAngle = def.orbitAngle;
	setRadius(def.radius);
	setAmplitude(def.amplitude);
	angle = ofRandom(0, 360);

	// Set color
	auto& color = getColorCycler();
	color.setConstantColor(def.color);

	// Setup ring
	ring.setParent(*this);
	ring.setMusicAnalysis(analysis);
	ring.setAmplitude(2);
	auto& ringColor = ring.getColorCycler();
	ringColor.setRainbow(1, 1);
	ringColor.setDuration(3.0);

	ring.setInnerRadius(getRadius() + .1);
	ring.setOuterRadius(ring.getInnerRadius() + (1.0 / level) * 5);
}

Orbital::Orbital(int level, int maxLevel, float radiusScalar, float rotationScalar, float speedScalar, MusicAnalysis* analysis)
{
	setMusicAnalysis(analysis);

	// Setup ring
	ring.setParent(*this);
	ring.setMusicAnalysis(analysis);
	ring.setAmplitude(2);
	auto& ringColor = ring.getColorCycler();
	ringColor.setRainbow(1, 1);
	ringColor.setDuration(3.0);
	
	ring.setInnerRadius(getRadius() + .1);
	ring.setOuterRadius(ring.getInnerRadius() + (1.0 / level) * 5);

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

void Orbital::update()
{
	Globe::update();
	setOrbitalPos();

	ring.setOrientation(ofVec3f(0, ofGetElapsedTimef() * 25.0, 10.0));
	ring.update();

	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->update();
	}
}

void Orbital::draw() const
{
	ofPushMatrix();
	ofRotate(orbitAngle);
	Globe::draw();
	ring.draw();
	ofPopMatrix();
}

void Orbital::customDraw()
{
	Globe::customDraw();

	for (int i = 0; i < children.size(); i++)
	{
		ofPath p;
		p.setCircleResolution(42);
		p.setFilled(false);
		p.setStrokeWidth(1);

		// Build orbit path
		float radius = children[i]->getPosition().distance(getPosition());
		p.moveTo(ofPoint(radius, 0));
		p.circle(ofPoint(), radius);

		// Draw path
		ofPushMatrix();
		ofTranslate(getPosition());
		ofRotateX(90);
		ofSetColor(255);
		p.draw();
		ofPopMatrix();

		children[i]->draw();
	}
}

void Orbital::setOrbitalPos()
{
	if (parent != NULL)
	{
//		ofVec3f parentPos = parent->getPosition();
		angle += speed * ofGetLastFrameTime();

		float x = sin(ofDegToRad(angle));
		float y = cos(ofDegToRad(angle));
		ofVec3f turnVec = ofVec3f(x, 0, y);

		ofVec3f offset = rotRadius * turnVec;
		//ofVec3f offset(50, 50, 0);
//		ofVec3f newPos = parentPos + offset;
//		setPosition(newPos);
		setPosition(offset);
	}
}

void Orbital::createOrbitals()
{
	for (int i = 0; i < numOfChildren; i++)
	{
		Orbital* orb = new Orbital(level+1, maxLevel, radiusScale, rotationScale, speedScale, getMusicAnalysis());
		orb->setParent(*this);
		children.push_back(orb);
	}
}
