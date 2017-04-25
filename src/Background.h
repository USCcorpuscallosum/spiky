#pragma once

#include <ofMain.h>
#include "ofCustomMaterial.h"

/*
  Control backgroud color based on information from the music and the terrain
*/
class Background {
public:
	Background();
	void update();
	void draw();

	void debugReload();

	void setMusicAnalysis(class MusicAnalysis* analysis) { mAnalysis = analysis; }

private:
	void setupMaterial();

	ofPlanePrimitive mPlane;
	ofShader mShader;

	class MusicAnalysis* mAnalysis;

};
