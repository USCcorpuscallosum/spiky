#include "ofApp.h"
#include "Terrain.h"
#include "MusicAnalysis.h"

ofApp* ofApp::sInstance = nullptr;
ofApp::ofApp() {
	sInstance = this;
}

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofEnableDepthTest();
//	ofSetBackgroundAuto(false); // don't clear the color buffer each frame
	ofSetSmoothLighting(true);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//	ofEnableAntiAliasing();

	light.setPointLight();
	light.setDiffuseColor(ofColor(255, 255, 255));
	light.setSpecularColor(light.getDiffuseColor());
	light.setPosition(0, 0, 40);
	light.setAttenuation(1, 0, 0);

	font.load("Cabin-Regular.ttf", 14);
	font.setLineHeight(14); 

	// Setup background
	background.setMusicAnalysis(&analysis);

	// Setup starfield
	starfield.setStarCount(250);
	starfield.setRadius(100.0);
	starfield.setMusicAnalysis(&analysis);

	// Setup terrain
	terrain.setPosition(ofVec3f(0, -40, 0));
	terrain.setMusicAnalysis(&analysis);

	// Setup flare
	flare.setMusicAnalysis(&analysis);
	flare.setInnerRadius(10.0);
	flare.setOuterRadius(flare.getInnerRadius() + 15.0);
	flare.setOpacity(0.8);

	// Build the galaxy
	auto galaxyDef = buildGalaxy();
	galaxy.initialize(galaxyDef, &analysis);
//	galaxy.initialize(2, 1.2, 30, 10, &analysis);

	// Load music and play
	songNames.push_back("bensound-dubstep.mp3");
	songNames.push_back("bensound-buddy.mp3");
	songNames.push_back("bensound-epic.mp3");
	songNames.push_back("bensound-happiness.mp3");
	songNames.push_back("bensound-littleidea.mp3");
	songNames.push_back("bensound-acousticbreeze.mp3");
	soundPlayers.resize(songNames.size());
	for (int i = 0; i < songNames.size(); i++) {
		soundPlayers[i].load(songNames[i]);
		soundPlayers[i].setLoop(true);
	}

	analysis.setPlayer(&soundPlayers[0]);
	nowPlaying = "Playing: " + songNames[0];

	// Print audio devices
	auto deviceNames = recordPlayer.getRecordingDeviceNames();
	ofLogNotice("ofApp") << "Found " << deviceNames.size() << " audio devices:";
	for (size_t i = 0; i < deviceNames.size(); i++) {
		ofLogNotice("ofApp") << ' ' << i << ": " << deviceNames[i];
	}
}

//--------------------------------------------------------------
void ofApp::update()
{
	//Start Camera
	cam.begin();
	
	// Revolve camera around center
	cam.setPosition(
		cos(ofGetElapsedTimef() * revolveSpeed) * revolveDistance,
		25,
		sin(ofGetElapsedTimef() * revolveSpeed) * revolveDistance);
	cam.lookAt(ofVec3f());

	analysis.update();

	starfield.update();
	terrain.update();
	flare.update();
	galaxy.update();

	// Shrink the FOV for a zoom effect on beats
	cam.setFov(60.0 - analysis.getDecayNormalized() * (lsd ? cameraFovShrinkLSD : cameraFovShrink));

	//End camera
	cam.end();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// Only clear the depth buffer each frame
//	glClear(GL_DEPTH_BUFFER_BIT);

	cam.begin();
	background.draw();

	ofEnableLighting();
	light.enable();

	starfield.draw();
	terrain.draw();
	flare.draw();
	galaxy.draw();

	ofDisableLighting();
	cam.end();

	drawUI();
}

void ofApp::drawUI() {
	ofDisableDepthTest();

	// Draw now playing
	const float MARGIN = 25, PADDING = 10;
	ofSetColor(0, 0, 0, 64);
	ofDrawRectangle(
		MARGIN,
		ofGetWindowHeight() - MARGIN - PADDING - font.getLineHeight() - PADDING,
		PADDING + font.stringWidth(nowPlaying) + PADDING,
		PADDING + font.getLineHeight() + PADDING);

	ofSetColor(255);
	font.drawString(nowPlaying, MARGIN + PADDING, ofGetWindowHeight() - MARGIN - PADDING - 2);

	ofEnableDepthTest();
}

Galaxy::OrbitalDef ofApp::buildGalaxy() {
	const int maxLevel = 2, numPlanets = 5, numMoons = 3;

	Galaxy::OrbitalDef sun;
	sun.radius = 10.0;
	sun.amplitude = 5.0;
	sun.color = ofColor(255, 213, 0);

	for (int p = 0; p < numPlanets; p++) {
		Galaxy::OrbitalDef planet;
		planet.orbitRadius = 40 + p * 30;
		planet.orbitInterval = 10 + 5 * p;
		if (ofRandomf() > 0) planet.orbitInterval *= -1; // backwards
		planet.orbitAngle = ofRandom(-5.0, 5.0);
		planet.radius = 6.0;
		planet.amplitude = 3.0;
		planet.color = ofFloatColor::fromHsb(ofRandom(1.0), 1, 1);
		planet.seed = ofRandom(1.0);

		for (int m = 0; m < numMoons; m++) {
			planet.children.emplace_back(buildGalaxyLevel(2, maxLevel));
		}

		sun.children.emplace_back(planet);
	}

	return sun;
}

Galaxy::OrbitalDef ofApp::buildGalaxyLevel(int level, int maxLevel) {
	const int numOfOrbitals = 2;
	const float radiusScale = 1.2, orbitRadiusScale = 30, orbitIntervalScale = 10;

	float levelMultiplier = pow(maxLevel + 1 - level, 2);

	Galaxy::OrbitalDef orb;
	orb.orbitRadius = levelMultiplier * orbitRadiusScale * ofRandom(.8f, 1.2f);
	orb.orbitInterval = (1 / levelMultiplier) * orbitIntervalScale * ofRandom(.8f, 1.2f);
	orb.orbitAngle = ofRandom(-15.0, 15.0);
	orb.radius = levelMultiplier * radiusScale * ofRandom(.8f, 1.2f) * .5;
	orb.amplitude = orb.radius * 0.5;
	orb.color = ofFloatColor::fromHsb(ofRandom(1.0), 1, 1);
	orb.seed = ofRandom(1.0);

	if (level < maxLevel) {
		for (int i = 0; i < numOfOrbitals; i++) {
			orb.children.emplace_back(buildGalaxyLevel(level + 1, maxLevel));
		}
	}

	return orb;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == ' ') {
		// Toggle play
		if (analysis.isPaused())
			analysis.play();
		else
			analysis.pause();
	} else if ('0' <= key && key <= '9') {
		// 1..9 = play song, 0 = play line in
		int index = key - '0';
		index--;
		if (index >= 0 && index < soundPlayers.size()) {
			analysis.setPlayer(&soundPlayers[index]);
			nowPlaying = "Playing: " + songNames[index];
		} else if (index < 0) {
			recordPlayer.record(AUDIO_DEVICE_NAME);
			analysis.setPlayer(&recordPlayer);
			nowPlaying = "Playing from phone";
		}
	} else if (key == 'r') {
		// Reload shaders and stuff
		background.debugReload();
		terrain.debugReload();
		flare.debugReload();
		galaxy.debugReload();
	} else if (key == 'l') {
		lsd = !lsd;
		background.setLSD(lsd);
		terrain.setLSD(lsd);
		starfield.setLSD(lsd);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) 
{
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}
