#include "ofApp.h"
#include "Terrain.h"
#include "MusicAnalysis.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofEnableDepthTest();
	ofSetBackgroundAuto(false); // don't clear the color buffer each frame
	ofSetSmoothLighting(true);
	ofEnableAlphaBlending();

	cam.setDistance(80);

	light.setPointLight();
	light.setDiffuseColor(ofColor(255, 255, 255));
	light.setSpecularColor(light.getDiffuseColor());
	light.setPosition(0, 0, 40);
	light.setAttenuation(1, 0, 0);

	// Setup background
	background.setTerrain(&terrain);
	background.setMusicAnalysis(&musicAnalysis);

	// Setup terrain
	terrain.setMusicAnalysis(&musicAnalysis);
	terrain.initializeTerrain();

	// Setup globe
	globe.setMusicAnalysis(&musicAnalysis);
	globe.setRadius(10);
	auto& globeColor = globe.getColorCycler();
	globeColor.mStartHue = 0.0;
	globeColor.mEndHue = 1.0;
	globeColor.mDuration = 10.0;

	// Setup ring
	ring.setMusicAnalysis(&musicAnalysis);
	ring.setInnerRadius(12);
	ring.setOuterRadius(25);
	ring.setAmplitude(4);
	auto& ringColor = ring.getColorCycler();
	ringColor.mStartHue = 0.0;
	ringColor.mEndHue = 1.0;
	ringColor.mRepeat = ColorCycler::PingPong;
	ringColor.mDuration = 3.0;

	// Load music and play
	songNames.push_back("bensound-dubstep.mp3");
	songNames.push_back("bensound-buddy.mp3");
	songNames.push_back("bensound-epic.mp3");
	songNames.push_back("bensound-happiness.mp3");
	songNames.push_back("bensound-littleidea.mp3");
	songNames.push_back("bensound-acousticbreeze.mp3");
	musicAnalysis.loadSongs(songNames);
	musicAnalysis.togglePlay();
}

//--------------------------------------------------------------
void ofApp::update()
{
	musicAnalysis.update();

	terrain.changeAllColors();
	terrain.changeHeight();

	globe.update();
	ring.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// Only clear the depth buffer each frame
	glClear(GL_DEPTH_BUFFER_BIT);

	background.draw();

	cam.begin();
	ofEnableLighting();
	light.enable();
	
//	light.draw();
	
	ofPushMatrix();
	ofRotateX(115);

	ofPushMatrix();
	ofTranslate(-50, -50, 10);
	terrain.draw();
	ofPopMatrix();

	globe.draw();
	ofPushMatrix();
	ofRotateY(15);
	ring.draw();
	ofPopMatrix();
	ofPopMatrix();

	cam.end();
	ofDisableLighting();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) 
{
	if (key == 'p')
	{
		musicAnalysis.togglePlay();
	}
	else if ('0' <= key && key <= '9')
	{
		int index = key - '0';
		if (index == 0) index = 10;
		index--;
		musicAnalysis.changeSong(index);
	}
	else if (key == 'r')
	{
		globe.debugReload();
		ring.debugReload();
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
