#pragma once

#include <ofMain.h>

class Starfield : public ofNode {
public:
	Starfield();
	void update();
	void customDraw() override;

private:
	void buildMesh();

	int mStarCount = 150;
	float mRadius = 50.0, mStarRadius = 0.6;

	ofVboMesh mMesh;

};
