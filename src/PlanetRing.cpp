#include "PlanetRing.h"
#include "MusicAnalysis.h"
#include "MaterialCache.h"

PlanetRing::PlanetRing() {
	setupMaterial(false);
}

void PlanetRing::update() {
	if (mIsMeshDirty) {
		buildMesh();
		mIsMeshDirty = false;
	}

	// Copy spectrum to a texture for the shader to use
	if (mAnalysis) {
		mRanges = mAnalysis->getHPCP();
		mRanges.emplace_back(mRanges[0]); // wrap
		mSpectrumTex.loadData(mRanges.data(), static_cast<int>(mRanges.size()), 1, GL_RED); // GL_RED is single value in red channel
	}
}

void PlanetRing::customDraw() {
	mMaterial->setDiffuseColor(mColorCycler.getColor());

	mMaterial->begin();

	auto& shader = mMaterial->getShader();
	shader.setUniform1f("amplitude", mAmplitude);
	shader.setUniform2f("ringSize", mRingSize);
	shader.setUniform2f("scrollDirection", mScrollDirection);
	shader.setUniform1f("fadeWidth", mFadeWidth);
	shader.setUniform1i("bins", static_cast<int>(mRanges.size()));
	if (mSpectrumTex.isAllocated()) shader.setUniformTexture("spectrum", mSpectrumTex, 1); // 1-indexed

	mMesh.draw();

	mMaterial->end();
}

void PlanetRing::debugReload() {
	setupMaterial(true);
	ofLogNotice("Ring") << "Reloaded shader";
}

void PlanetRing::setupMaterial(bool reload) {
	mMaterial = MaterialCache::load("shaders/ring/ring", reload);
	mMaterial->setShininess(127);
	mMaterial->setSpecularColor(ofColor(255, 255, 255));
}

void PlanetRing::buildMesh() {
	mMesh.clear();

	float deltaAngle = TWO_PI / RESOLUTION;
	ofIndexType offset = 0;

	for (int i = 0; i <= RESOLUTION; i++) {
		// Inner vert
		float x = mInnerRadius * cos(i * deltaAngle);
		float z = mInnerRadius * sin(i * deltaAngle);
		mMesh.addVertex(ofVec3f(x, 0, z));
		mMesh.addNormal(ofVec3f(0.0, 1.0, 0));
		mMesh.addTexCoord(ofVec2f(static_cast<float>(i) / RESOLUTION, 0.0));

		// Outer vert
		x = mOuterRadius * cos(i * deltaAngle);
		z = mOuterRadius * sin(i * deltaAngle);
		mMesh.addVertex(ofVec3f(x, 0, z));
		mMesh.addNormal(ofVec3f(0.0, 1.0, 0.0));
		mMesh.addTexCoord(ofVec2f(static_cast<float>(i) / RESOLUTION, 1.0));

		if (i < RESOLUTION) {
			mMesh.addIndex(offset);
			mMesh.addIndex(offset + 3);
			mMesh.addIndex(offset + 1);
			mMesh.addIndex(offset);
			mMesh.addIndex(offset + 2);
			mMesh.addIndex(offset + 3);
		}
		offset += 2;
	}
}
