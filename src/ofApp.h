#pragma once

#include <ofMain.h>
#include <vector>
#include "Terrain.h"
#include "Flare.h"
#include "MusicAnalysis.h"
#include "Background.h"
#include "Starfield.h"
#include "Galaxy.h"

class ofApp : public ofBaseApp {
public:
	ofApp();
	void setup() override;
	void update() override;
	void draw() override;
	void keyPressed(int key) override;

	ofCamera& getCamera() { return cam; }

	static ofApp* getInstance() { return sInstance; }

	const string AUDIO_DEVICE_NAME = "USB Audio Device";

private:
	void drawUI() const;

	static Galaxy::OrbitalDef buildGalaxy();
	static Galaxy::OrbitalDef buildGalaxyLevel(int level, int maxLevel, ofFloatColor baseColor);

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

	float cameraFovShrink = 3.0, cameraFovShrinkLsd = -10.0; // degrees
	float fovEasing = 0.6;
	float revolveSpeed = -0.15;
	float revolveDistance = 120.0;
	bool lsd = false;

	static ofApp* sInstance;

	Galaxy galaxy;

};
