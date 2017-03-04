#pragma once

#include "ofMain.h"

class Terrain
{
public:
	Terrain(musicAnalysis*);
	void changeColor();
	void changeHeihgt();
	void draw();
    void getColor();
private:
	ofMesh mesh;
	MusicAnalysis* musicAnalysis;
    ofColor color;
};
