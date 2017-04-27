#pragma once

#include <ofMain.h>
#include <ofxAssimpModelLoader.h>
#include "ofCustomMaterial.h"
#include "ColorCycler.h"

class Globe : public ofNode {
public:
	Globe();
	virtual void update();
	void customDraw() override;

	void debugReload();

	float getRadius() { return radius; }
	void setRadius(float radius_) { radius = radius_; }
	ofVec3f getCenter() { return center; }
	void setCenter(ofVec3f center_) { center = center_; }
	float getAmplitude() { return amplitude; }
	void setAmplitude(float amplitude_) { amplitude = amplitude_; }

	ColorCycler& getColorCycler() { return colorCycler; }
	class MusicAnalysis* getMusicAnalysis() const { return analysis; }
	void setMusicAnalysis(class MusicAnalysis* analysis_) { analysis = analysis_; }

	const float CYCLE_SPEED = 0.1;

private:
	void setupMaterial(bool reload);
	void calcVertexFrequencies();

	float radius = .8;
	ofVec3f center;
	float amplitude = 1.5;
	float colorHueRange = 0.05;
	ColorCycler colorCycler;

	ofVboMesh mesh;
	ofCustomMaterial* material = nullptr;
	bool didSetVertFrequencies = false;

	class MusicAnalysis* analysis = nullptr;
	vector<float> ranges;
	ofTexture spectrumTex;

	static ofxAssimpModelLoader modelLoader;

};
