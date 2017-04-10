#pragma once

#include <ofMain.h>
#include "ofCustomMaterial.h"
#include "ColorCycler.h"

class PlanetRing {
public:
	PlanetRing();
	void update();
	void draw();
	
	void debugReload();

	float getInnerRadius() { return mInnerRadius; }
	float setInnerRadius(float radius) { mInnerRadius = radius; mIsMeshDirty = true; }
	float getOuterRadius() { return mOuterRadius; }
	float setOuterRadius(float radius) { mOuterRadius = radius; mIsMeshDirty = true; }
	float getAmplitude() { return mAmplitude; }
	float setAmplitude(float amplitude) { mAmplitude = amplitude; mIsMeshDirty = true; }

	ColorCycler& getColorCycler() { return mColorCycler; }
	void setMusicAnalysis(class MusicAnalysis* analysis) { mAnalysis = analysis; }

	const unsigned int RESOLUTION = 30;

private:
	void setupMaterial();
	void buildMesh();

	float mInnerRadius = 1.0, mOuterRadius = 2.0, mAmplitude = 1.0;
	ofVec2f mRingSize = ofVec2f(10, 20);
	ofVec2f mScrollDirection = ofVec2f(0.5, -3.0);
	float mFadeWidth = 0.2;
	ColorCycler mColorCycler;

	ofVboMesh mMesh;
	bool mIsMeshDirty = true;
	ofCustomMaterial mMaterial;

	class MusicAnalysis* mAnalysis = nullptr;
	vector<float> mRanges;
	ofTexture mSpectrumTex;

};
