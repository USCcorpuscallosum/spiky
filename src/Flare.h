#pragma once

#include <ofMain.h>
#include "ColorCycler.h"
#include "ofCustomMaterial.h"

class Flare : public ofNode {
public:
	Flare();
	void update();
	void customDraw() override;

	void debugReload();

	float getInnerRadius() const { return mInnerRadius; }
	void setInnerRadius(float radius) { mInnerRadius = radius; }
	float getOuterRadius() const { return mOuterRadius; }
	void setOuterRadius(float radius) { mOuterRadius = radius; }
	float getOpacity() const { return mOpacity; }
	void setOpacity(float opacity) { mOpacity = opacity; }

	ColorCycler& getColorCycler() { return mColorCycler; }
	void setMusicAnalysis(class MusicAnalysis* analysis) { mAnalysis = analysis; }

private:
	void setupMaterial();

	float mInnerRadius = 1.0, mOuterRadius = 2.0;
	ColorCycler mColorCycler;
	float mOpacity = 1.0;

	ofPlanePrimitive mMesh;
	ofCustomMaterial mMaterial;

	class MusicAnalysis* mAnalysis = nullptr;

};
