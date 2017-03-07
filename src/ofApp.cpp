#include "ofApp.h"
#include "Terrain.h"
#include "MusicAnalysis.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	songNames.push_back("bensound-dubstep.mp3");
	songNames.push_back("bensound-cute.mp3");
	songNames.push_back("bensound-buddy.mp3");
	songNames.push_back("bensound-epic.mp3");
	songNames.push_back("bensound-happiness.mp3");
	songNames.push_back("bensound-littleidea.mp3");
	songNames.push_back("bensound-acousticbreeze.mp3");
	
	musicAnalysis.loadSongs(songNames);

	globe.setMusicAnalysis(&musicAnalysis);
	globe.initializeTerrain();

	terrain.setMusicAnalysis(&musicAnalysis);
	terrain.initializeTerrain();

	background.setTerrain(&terrain);
	background.setMusicAnalysis(&musicAnalysis);

	musicAnalysis.togglePlay();
	cam.setDistance(150);

	ofSetBackgroundAuto(false); // don't clear the color buffer each frame
	ofSetVerticalSync(true);
	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update()
{
	terrain.changeAllColors();
	terrain.changeHeight();

	globe.changeAllColors();
	//globe.changeHeight();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// Only clear the depth buffer each frame
	glClear(GL_DEPTH_BUFFER_BIT);

	background.draw();

	cam.begin();
	
	ofPushMatrix();
	ofTranslate(-50, -50, 0);
	terrain.draw();
	//globe.draw();
	ofPopMatrix();

	cam.end();
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
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}
