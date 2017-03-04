#pragma once

#include "ofMain.h"

class Terrain
{
public:
	Terrain(musicAnalysis*);
	void changeColor();
	void changeHeihgt();
	void draw();
private:
	ofMesh mesh;
	MusicAnalysis* musicAnalysis;
};