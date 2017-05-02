#pragma once

#include <ofMain.h>
#include "ofCustomMaterial.h"
#include "ColorCycler.h"

class PlanetRing : public ofNode {
public:
	PlanetRing();
	void update();
	void customDraw() override;
	
	void debugReload();

	float getInnerRadius() const { return mInnerRadius; }
	void setInnerRadius(float radius) { mInnerRadius = radius; mIsMeshDirty = true; }
	float getOuterRadius() const { return mOuterRadius; }
	void setOuterRadius(float radius) { mOuterRadius = radius; mIsMeshDirty = true; }
	float getAmplitude() const { return mAmplitude; }
	void setAmplitude(float amplitude) { mAmplitude = amplitude; mIsMeshDirty = true; }

	ColorCycler& getColorCycler() { return mColorCycler; }
	void setMusicAnalysis(class MusicAnalysis* analysis) { mAnalysis = analysis; }

	const unsigned int RESOLUTION = 30;

private:
	void setupMaterial(bool reload);
	void buildMesh();

	float mInnerRadius = 1.0, mOuterRadius = 2.0, mAmplitude = 1.0;
	ofVec2f mRingSize = ofVec2f(10, 20);
	ofVec2f mScrollDirection = ofVec2f(0.5, -3.0);
	float mFadeWidth = 0.2;
	ColorCycler mColorCycler;

	ofVboMesh mMesh;
	bool mIsMeshDirty = true;
	ofCustomMaterial* mMaterial = nullptr;

	class MusicAnalysis* mAnalysis = nullptr;
	vector<float> mRanges;
	ofTexture mSpectrumTex;

};
