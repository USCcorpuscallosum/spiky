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

	// Setup background
	background.setTerrain(&terrain);
	background.setMusicAnalysis(&analysis);

	// Setup terrain
	terrain.setPosition(ofVec3f(0, -20, 0));
	terrain.setMusicAnalysis(&analysis);

	// Setup globe
	globe.setMusicAnalysis(&analysis);
	globe.setRadius(10.0);
	globe.setAmplitude(5.0);
	auto& globeColor = globe.getColorCycler();
	globeColor.mStartHue = 0.0;
	globeColor.mEndHue = 1.0;
	globeColor.mDuration = 10.0;

	// Setup ring
	ring.setMusicAnalysis(&analysis);
	ring.setInnerRadius(12);
	ring.setOuterRadius(25);
	ring.setAmplitude(4);
	auto& ringColor = ring.getColorCycler();
	ringColor.mStartHue = 0.0;
	ringColor.mEndHue = 1.0;
	ringColor.mRepeat = ColorCycler::PingPong;
	ringColor.mDuration = 3.0;

	// Setup flare
	flare.setMusicAnalysis(&analysis);
	flare.setInnerRadius(globe.getRadius());
	flare.setOuterRadius(globe.getRadius() + 7.0);
	flare.setOpacity(0.8);

	// Load music and play
	songNames.push_back("bensound-dubstep.mp3");
	songNames.push_back("bensound-buddy.mp3");
	songNames.push_back("bensound-epic.mp3");
	songNames.push_back("bensound-happiness.mp3");
	songNames.push_back("bensound-littleidea.mp3");
	songNames.push_back("bensound-acousticbreeze.mp3");
	analysis.loadSongs(songNames);

	analysis.play();
}

//--------------------------------------------------------------
void ofApp::update()
{
	// Revolve camera around center
	cam.setPosition(
		cos(ofGetElapsedTimef() * revolveSpeed) * revolveDistance,
		20,
		sin(ofGetElapsedTimef() * revolveSpeed) * revolveDistance);
	cam.lookAt(ofVec3f());

	analysis.update();

	terrain.update();
	globe.update();
	ring.setOrientation(ofVec3f(0, ofGetElapsedTimef() * 25.0, 10.0));
	ring.update();
	flare.update();

	// Shrink the FOV for a zoom effect on beats
	cam.setFov(60.0 - analysis.getDecayNormalized() * cameraFovShrink);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// Only clear the depth buffer each frame
//	glClear(GL_DEPTH_BUFFER_BIT);

	background.draw();

	cam.begin();
	ofEnableLighting();
	light.enable();

	starfield.draw();
	terrain.draw();
	globe.draw();
	flare.draw();
	ring.draw();

	ofDisableLighting();
	cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) 
{
	if (key == ' ')
	{
		// Toggle play
		if (analysis.isPaused())
			analysis.play();
		else
			analysis.pause();
	}
	else if ('0' <= key && key <= '9')
	{
		// 1..9 = play song, 0 = play line in
		int index = key - '0';
		index--;
		if (index >= 0)
			analysis.setSong(index);
		else
			analysis.setDeviceId(LINE_IN_DEVICE_ID);
	}
	else if (key == 'r')
	{
		// Reload shaders and stuff
		globe.debugReload();
		ring.debugReload();
		flare.debugReload();
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
void ofApp::mousePressed(int x, int y, int button) {

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
