#pragma once

#include "ofMain.h"
#include "Terrain.h"
#include "Globe.h"
#include "PlanetRing.h"
#include "MusicAnalysis.h"
#include "Background.h"

#include <vector>

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

private:
	ofCamera cam;
	ofLight light;

	MusicAnalysis analysis;
	Globe globe;
	PlanetRing ring;
	Terrain terrain;
	Background background;
	vector<string> songNames;

	float revolveSpeed = -0.1;
	float revolveDistance = 50.0;

};
