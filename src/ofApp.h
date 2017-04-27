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
#include "Galaxy.h"

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

	const string AUDIO_DEVICE_NAME = "USB Audio Device";

private:
	void drawUI();

	static Galaxy::OrbitalDef buildGalaxy();
	static Galaxy::OrbitalDef buildGalaxyLevel(int level, int maxLevel);

	ofCamera cam;
	ofLight light;
	ofTrueTypeFont font;

	vector<string> songNames;
	vector<ofFmodSoundPlayerExtended> soundPlayers;
	ofFmodSoundPlayerExtended recordPlayer;
	MusicAnalysis analysis;
	string nowPlaying;

	Flare flare;
	Terrain terrain;
	Background background;
	Starfield starfield;

	float cameraFovShrink = 3.0, cameraFovShrinkLSD = -10.0; // degrees
	float revolveSpeed = -0.15;
	float revolveDistance = 120.0;
	bool lsd = false;

	static ofApp* sInstance;

	Galaxy galaxy;

	//ofEasyCam myCam;

};
