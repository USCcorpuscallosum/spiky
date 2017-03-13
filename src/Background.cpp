#include "Background.h"

Background::Background()
{
	shader.load("shaders/background/lsd");

	// Create the quad that the shader will be applied on
	plane.set(2, 2, 2, 2);
}

void Background::draw()
{
	shader.begin();
	ofDisableDepthTest(); // treat this as the background, not another solid object
	ofEnableAlphaBlending();

	float volume = musicAnalysis->getDecayNormalized();

	shader.setUniform4f("terrainColor", terrain->getColor());
	shader.setUniform1f("time", ofGetElapsedTimef());
	shader.setUniform1f("volume", volume);
	plane.draw();

	ofDisableAlphaBlending();
	ofEnableDepthTest(); // reset
	shader.end();
}
