#include "Globe.h"
#include "MusicAnalysis.h"
#include "MaterialCache.h"

ofxAssimpModelLoader Globe::modelLoader;

Globe::Globe() {
	if (modelLoader.getMeshCount() == 0) {
		modelLoader.loadModel("Sphere5.obj");
	}
	mesh = modelLoader.getMesh(0);

	setupMaterial(false);
}

void Globe::update() {
	// Copy spectrum to a texture for the shader to use
	if (analysis) {
		ranges = analysis->getHPCP();
		spectrumTex.loadData(ranges.data(), static_cast<int>(ranges.size()), 1, GL_RED); // GL_RED is single value in red channel

		if (!didSetVertFrequencies) {
			calcVertexFrequencies();
			didSetVertFrequencies = true;
		}
	}
}

void Globe::customDraw() {
	material->begin();
	material->setDiffuseColor(colorCycler.getColor());

	auto& shader = material->getShader();
	shader.setUniform1f("amplitude", amplitude / radius); // amplitude is relative to scale
	shader.setUniform1f("colorHueRange", colorHueRange);
	shader.setUniform1i("bins", static_cast<int>(ranges.size()));
	if (spectrumTex.isAllocated()) shader.setUniformTexture("spectrum", spectrumTex, 1); // 1-indexed

	ofPushMatrix();
	ofScale(radius, radius, radius);
	ofTranslate(center);
	mesh.draw();
	ofPopMatrix();

	material->end();
}

void Globe::debugReload() {
	setupMaterial(true);
	calcVertexFrequencies();
	ofLogNotice("Globe") << "Reloaded shader";
}

void Globe::setupMaterial(bool reload) {
	material = MaterialCache::load("shaders/globe/globe", reload);
	material->setShininess(127);
	material->setSpecularColor(ofColor(255, 255, 255));
}

/** Figure out the frequency to use for each vert in the range 0..1 */
void Globe::calcVertexFrequencies() {
	const float NOISE_SCALE = 200;

	std::vector<float> vertFrequency(mesh.getNumVertices());
	for (int i = 0; i < mesh.getNumVertices(); i++) {
		ofVec3f pos = mesh.getVertex(i);
		vertFrequency[i] = ofNoise(pos * NOISE_SCALE) * ranges.size(); // needs to be between 0 and spectrum texture width
	}

	// Link the frequencies to the mesh
	auto& shader = material->getShader();
	GLint freqAttrib = shader.getAttributeLocation("frequency");
	mesh.getVbo().setAttributeData(freqAttrib, vertFrequency.data(), 1, static_cast<int>(mesh.getNumVertices()), GL_STATIC_DRAW, sizeof(float));
}
