#include "ofApp.h"
#include "Terrain.h"
#include "MusicAnalysis.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	songNames.push_back("song_1.wav");
	
	musicAnalysis.loadSongs(songNames);
	terrain.setMusicAnalysis(&musicAnalysis);
	terrain.initializeTerrain();

	cam.setDistance(50);

	ofSetVerticalSync(true);
	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update()
{
	terrain.changeHeight();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	cam.begin();
	
	terrain.draw();

	cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

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
