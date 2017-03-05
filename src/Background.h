#pragma once

#include "ofMain.h"
#include "MusicAnalysis.h"
#include "Terrain.h"

class Terrain;
class MusicAnalysis;

/*
  Control backgroud color based on information from the music and the terrain
*/
class Background
{
public:
	Background();	// Default constructor
	void changeBackground();	// Change the color of the background
	void setTerrain(Terrain* t) { terrain = t; }	// Set the foreground terrain

private:
	ofColor color;	// Background color
	ofImage backgroundImage;	// Background image (not implemented yet)
	MusicAnalysis* musicAnalysis;	// Pointer to the music controler
	Terrain* terrain;	// Pointer to the terrain on the foreground

};
