#pragma once

#include <ofMain.h>

class Starfield : public ofNode {
public:
	void update();
	void customDraw() override;

	void setLSD(bool on) { mLsd = on; }

	int getStarCount() const { return mStarCount; }
	void setStarCount(int starCount) { mStarCount = starCount; }
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
	bool mLsd = false;

	ofVboMesh mMesh;

	class MusicAnalysis* mAnalysis = nullptr;

};
