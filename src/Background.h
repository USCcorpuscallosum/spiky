#pragma once

#include <ofMain.h>
#include "ColorCycler.h"

/*
  Control backgroud color based on information from the music and the terrain
*/
class Background {
public:
	Background();
	void draw();

	void debugReload();
	void setLSD(bool on) { mMode = on ? 1 : 0; }

	void setMusicAnalysis(class MusicAnalysis* analysis) { mAnalysis = analysis; }

private:
	void setupMaterial();

	ofPlanePrimitive mPlane;
	ofShader mShader;
	ColorCycler mColorCycler;
	int mMode = 0;

	class MusicAnalysis* mAnalysis;

};
