#include "Background.h"

Background::Background()
{
	shader.load("shaders/background/color");

	// Create the quad that the shader will be applied on
	plane.set(2, 2, 2, 2);
}

void Background::draw()
{
	shader.begin();
	ofDisableDepthTest(); // treat this as the background, not another solid object

	shader.setUniform4f("terrainColor", terrain->getColor());
	plane.draw();

	ofEnableDepthTest(); // reset
	shader.end();
}
