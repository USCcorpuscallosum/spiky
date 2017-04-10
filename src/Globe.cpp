#include "Globe.h"
#include "MusicAnalysis.h"

Globe::Globe()
{
	modelLoader.loadModel("Sphere5.obj");
	mesh = modelLoader.getMesh(0);

	material.load("shaders/globe/globe");
	material.setShininess(127);
	material.setSpecularColor(ofColor(255, 255, 255));
}

void Globe::update()
{
	material.setDiffuseColor(colorCycler.getColor());

	// Copy spectrum to a texture for the shader to use
	if (analysis)
	{
		ranges = analysis->getHPCP();
		spectrumTex.loadData(ranges.data(), ranges.size(), 1, GL_RED); // GL_RED is single value in red channel

		if (!didSetVertFrequencies)
		{
			calcVertexFrequencies();
			didSetVertFrequencies = true;
		}
	}
}

void Globe::draw()
{
	material.begin();

	auto& shader = material.getShader();
	shader.setUniform1f("amplitude", amplitude);
	shader.setUniform1f("colorHueRange", colorHueRange);
	shader.setUniform1i("bins", ranges.size());
	if (spectrumTex.isAllocated()) shader.setUniformTexture("spectrum", spectrumTex, 1); // 1-indexed

	ofPushMatrix();
	ofScale(radius, radius, radius);
	ofTranslate(center);
	mesh.draw();
	ofPopMatrix();

	material.end();
}

void Globe::debugReload()
{
	material = ofCustomMaterial();
	material.load("shaders/globe/globe");
	calcVertexFrequencies();
	ofLogNotice("Globe") << "Reloaded shader";
}

/** Figure out the frequency to use for each vert in the range 0..1 */
void Globe::calcVertexFrequencies()
{
	const float NOISE_SCALE = 200;

	std::vector<float> vertFrequency(mesh.getNumVertices());
	for (size_t i = 0; i < mesh.getNumVertices(); i++)
	{
		ofVec3f pos = mesh.getVertex(i);
		vertFrequency[i] = ofNoise(pos * NOISE_SCALE) * ranges.size(); // needs to be between 0 and spectrum texture width
	}

	// Link the frequencies to the mesh
	auto& shader = material.getShader();
	GLint freqAttrib = shader.getAttributeLocation("frequency");
	mesh.getVbo().setAttributeData(freqAttrib, vertFrequency.data(), 1, mesh.getNumVertices(), GL_STATIC_DRAW, sizeof(float));
}
