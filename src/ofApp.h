#pragma once

#include <ofMain.h>
#include <vector>
#include "Terrain.h"
#include "Globe.h"
#include "PlanetRing.h"
#include "Flare.h"
#include "MusicAnalysis.h"
#include "Background.h"
#include "Starfield.h"

class ofApp : public ofBaseApp {
public:
	ofApp();
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

	ofCamera& getCamera() { return cam; }

	static ofApp* getInstance() { return sInstance; }

	const int LINE_IN_DEVICE_ID = 1;

private:
	ofCamera cam;
	ofLight light;

	MusicAnalysis analysis;
	Globe globe;
	PlanetRing ring;
	Flare flare;
	Terrain terrain;
	Background background;
	Starfield starfield;
	vector<string> songNames;

	float cameraFovShrink = 2.0; // degrees
	float revolveSpeed = -0.1;
	float revolveDistance = 50.0;

	static ofApp* sInstance;

};
