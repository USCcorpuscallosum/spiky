#pragma once

#include <ofMain.h>
#include <ofxAssimpModelLoader.h>
#include "ofCustomMaterial.h"

class Globe
{
public:
	Globe();
	void update();
	void draw();

	void debugReload();

	float getRadius() { return radius; }
	void setRadius(float radius_) { radius = radius_; }
	ofVec3f getCenter() { return center; }
	void setCenter(ofVec3f center_) { center = center_; }
	float getAmplitude() { return amplitude; }
	void setAmplitude(float amplitude_) { amplitude = amplitude_; }

	void setMusicAnalysis(class MusicAnalysis* analysis_) { analysis = analysis_; }

	const float CYCLE_SPEED = 0.1;

private:
	void calcVertexFrequencies();

	float radius = 1;
	ofVec3f center;
	float amplitude = 1;
	class MusicAnalysis* analysis = nullptr;
	float hue = -1, saturation, brightness;
	bool didSetVertFrequencies = false;

	ofxAssimpModelLoader modelLoader;
	ofVboMesh mesh;
	ofCustomMaterial material;
	vector<float> ranges;
	ofTexture spectrumTex;
};
