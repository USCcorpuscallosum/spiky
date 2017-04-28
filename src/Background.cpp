#include "Background.h"
#include "MusicAnalysis.h"

Background::Background() {
	setupMaterial();

	// Create the quad that the shader will be applied on
	mPlane.set(2, 2, 2, 2);

	mColorCycler.setRainbow(1, 1);
	mColorCycler.setDuration(15.0);
}

void Background::update() {
}

void Background::draw() {
	mShader.begin();
	ofDisableDepthTest(); // treat this as the background, not another solid object

	float volume = mAnalysis->getDecayNormalized();

	mShader.setUniform1i("mode", mMode);
	mShader.setUniform4f("terrainColor", mColorCycler.getColor());
	mShader.setUniform1f("time", ofGetElapsedTimef());
	mShader.setUniform1f("volume", volume);
	mPlane.draw();

	ofEnableDepthTest(); // reset
	mShader.end();
}

void Background::debugReload() {
	setupMaterial();
	ofLogNotice("Background") << "Reloaded shader";
}

void Background::setupMaterial() {
	mShader = ofShader();
	mShader.load("shaders/background/color");
}
