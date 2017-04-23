#pragma once

#include <ofMain.h>

class Starfield : public ofNode {
public:
	Starfield();
	void update();
	void customDraw() override;

	float getRadius() const { return mRadius; }
	void setRadius(float radius) { mRadius = radius; }
	float getStarRadius() const { return mStarRadius; }
	void setStarRadius(float radius) { mStarRadius = radius; }

	void setMusicAnalysis(class MusicAnalysis* analysis) { mAnalysis = analysis; }

private:
	void buildMesh();

	int mStarCount = 150;
	float mRadius = 50.0, mStarRadius = 0.6;
	float mLightThreshold = 0.9;

	ofVboMesh mMesh;

	class MusicAnalysis* mAnalysis = nullptr;

};
