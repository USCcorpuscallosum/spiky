#include "ofApp.h"
#include "Terrain.h"
#include "MusicAnalysis.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	songNames.push_back("382965__martysonic__sweet-angels-dance.wav");
	
	musicAnalysis.loadSongs(songNames);
	terrain.setMusicAnalysis(&musicAnalysis);
	terrain.initializeTerrain();
	background.setTerrain(&terrain);

	musicAnalysis.togglePlay();
	cam.setDistance(50);

	ofSetVerticalSync(true);
	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update()
{
	terrain.changeAllColors();
	terrain.changeHeight();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	background.changeBackground();

	cam.begin();
	
	ofPushMatrix();
	ofTranslate(-50, -50, 0);
	terrain.draw();
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
