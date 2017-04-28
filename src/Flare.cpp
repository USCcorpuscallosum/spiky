#include "Flare.h"
#include "MusicAnalysis.h"
#include "ofApp.h"

Flare::Flare() {
	// Create quad
	mMesh.set(2, 2, 2, 2);

	setupMaterial();
}

void Flare::update() {
	setScale(mOuterRadius);

	// Billboard
	ofCamera& camera = ofApp::getInstance()->getCamera();
	lookAt(camera.getPosition(), ofVec3f(0, 1, 0));
}

void Flare::customDraw() {
	float volume = mAnalysis ? mAnalysis->getDecayNormalized() : 0;

	mMaterial.begin();

	ofFloatColor color = mColorCycler.getColor();
	color.a = mOpacity;
	mMaterial.setDiffuseColor(color);

	auto& shader = mMaterial.getShader();
	shader.setUniform1f("volume", volume);
	shader.setUniform1f("centerRadius", mInnerRadius / mOuterRadius);

	glDepthMask(false); // disable depth write but keep depth testing
	mMesh.draw();
	glDepthMask(true);

	mMaterial.end();
}

void Flare::debugReload() {
	setupMaterial();
	ofLogNotice("Flare") << "Reloaded shader";
}

void Flare::setupMaterial() {
	mMaterial = ofCustomMaterial();
	mMaterial.load("shaders/flare/flare");
}
