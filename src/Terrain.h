#pragma once

#include "ofMain.h"

class Terrain
{
public:
	Terrain(musicAnalysis*);
	changeColor();
	changeHeihgt();
	draw();
private:
	ofMesh mesh;
	MusicAnalysis* musicAnalysis;
};