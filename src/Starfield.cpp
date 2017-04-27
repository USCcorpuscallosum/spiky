#include "Starfield.h"
#include "MusicAnalysis.h"

void Starfield::update() {
	if (mMesh.getNumVertices() == 0) {
		buildMesh();
	}
}

void Starfield::customDraw() {
	ofSetColor(255);
	if (mAnalysis->getDecayNormalized() > mLightThreshold)
		mMesh.draw(); // fill
	else
		mMesh.drawWireframe();
}

static ofVec3f randomInsideUnitSphere() {
	// From http://stackoverflow.com/a/5408843/1248884
	float phi = ofRandom(0, TWO_PI);
	float costheta = ofRandom(-1, 1);
	float u = ofRandom(0, 1);
	float theta = acos(costheta);
	float r = pow(u, 0.33333333);

	return ofVec3f(
		r * sin(theta) * cos(phi),
		r * sin(theta) * sin(phi),
		r * cos(theta)
   );
}

void Starfield::buildMesh() {
	ofIndexType offset = 0;
	for (int i = 0; i < mStarCount; i++) {
		ofVec3f center = randomInsideUnitSphere() * mRadius;
		//center.y = abs(center.y); // make hemisphere above y = 0

		ofVec3f normal = ofVec3f(ofRandomf(), ofRandomf(), ofRandomf());
		normal.normalize();
		ofMatrix4x4 rotToNormal;
		rotToNormal.makeRotationMatrix(ofVec3f(0, 0, 1), normal);

		float angle = ofRandom(0, TWO_PI);
		for (int vert = 0; vert < 3; vert++) {
			ofVec3f pos = ofVec3f(cos(angle) * mStarRadius, sin(angle) * mStarRadius, 0);
			pos = rotToNormal * pos;
			mMesh.addVertex(center + pos);
			mMesh.addIndex(offset);
			switch (vert) {
				case 0: mMesh.addTexCoord(ofVec2f(0, 0)); break;
				case 1: mMesh.addTexCoord(ofVec2f(1, 0)); break;
				case 2: mMesh.addTexCoord(ofVec2f(1, 1)); break;
			}

			angle += TWO_PI / 3;
			offset++;
		}
	}
}
