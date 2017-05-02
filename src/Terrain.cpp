#include "Terrain.h"
#include <ofMain.h>
#include "MusicAnalysis.h"
#include "MapGenerator/Map.h"

Terrain::Terrain()
{
	randomPosScalar = .2;
	
	width = 100;
	length = 100;
	skip = 2;

	colorCycler.setRainbow(1, 1);
	colorCycler.setDuration(CYCLE_SPEED);

	setupMaterial();

	map = new Map(width, length, 4, ofGetUnixTime());
	map->Generate();
	initializeTerrain();
}

Terrain::~Terrain()
{
	delete map;
}

void Terrain::update()
{
	updateHeight();
}

void Terrain::customDraw()
{
	material.setDiffuseColor(colorCycler.getColor());

	material.begin();

	auto& shader = material.getShader();
	shader.setUniform1f("edgeDistance", min(width, length) * skip * 0.5);
	shader.setUniform1i("mode", mode);

	mesh.draw();

	material.end();
}

void Terrain::debugReload()
{
	setupMaterial();
	ofLogNotice("Terrain") << "Reloaded shader";
}

void Terrain::setupMaterial()
{
	material = ofCustomMaterial();
	material.load("shaders/terrain/gradient1");
}

//Sets up all the vertices for the mesh
void Terrain::initializeTerrain()
{
	//Create vertices
	for (int z = 0; z < length; z++)
	{
		for (int x = 0; x<width; x++) 
		{
			float elevation = map->GetCenterAt(Vec2(x, z))->elevation;
			mesh.addVertex(ofPoint(
				(x - width * 0.5) * skip,
				elevation * (ELEVATION_MAX - ELEVATION_MIN) + ELEVATION_MIN,
				(z - length * 0.5) * skip)); // mesh index = x + z*width

			mesh.addNormal(ofVec3f(ofRandomf() / 5, 1, ofRandomf() / 5));
//			mesh.addColor(ofFloatColor(0, ofRandom(0.7, 1.0), ofRandom(0.5, 1.0))); // pass in HSB
			mesh.addColor(ofFloatColor(floor(elevation * ELEVATION_STEPS), 0, 0));

			mesh.addTexCoord(ofVec2f(x % 2, z % 2));
		}
	}

	//Create indices
	for (int z = 0; z < length - 1; z++)
	{
		for (int x = 0; x < width - 1; x++)
		{
			mesh.addIndex(x + z*width);
			mesh.addIndex((x + 1) + z*width);
			mesh.addIndex(x + (z + 1)*width);

			mesh.addIndex((x + 1) + z*width);
			mesh.addIndex((x + 1) + (z + 1)*width);
			mesh.addIndex(x + (z + 1)*width);
		}
	}

	baseMesh = mesh;
}

//Height changes based on frequency
void Terrain::updateHeight()
{
	const float BASS_HEIGHT = 10.f;
	const float BASS_SCALE = 0.04f; // smaller means larger regions
	const ofVec2f BASS_SPEED(0.5f, 0.2f);
	const float MID_HEIGHT = 5.f;
	const float MID_SCALE = 0.4f;
	const ofVec2f MID_SPEED(0.2f, -0.5f);

	float time = ofGetElapsedTimef();

	auto& bass = musicAnalysis->getRange(MusicAnalysis::RANGE_BASS);
	auto& mid = musicAnalysis->getRange(MusicAnalysis::RANGE_MID);

	/*for (int i = 0; i < mesh.getNumIndices(); i++)
	{
		ofVec2f position = mesh.getVertex(i);

		float newOffset = 0;
		newOffset += ofNoise(position * BASS_SCALE + BASS_SPEED * time) * bass / bassMax * BASS_HEIGHT;
		newOffset += ofNoise(position * MID_SCALE + MID_SPEED  * time) * mid / midMax  * MID_HEIGHT;

		calculateNewPosForIndex(i, newOffset);
	}*/

	for (int z = 0; z < length; z++)
	{
		for (int x = 0; x < width; x++)
		{
			int index = z * length + x;
			ofVec2f position(x, z);

			// Calculate the new height of each vertex from the audio
			float newOffset = 0;
			newOffset += ofNoise(position * BASS_SCALE + BASS_SPEED * time) * bass.volume / bass.maxVolume * BASS_HEIGHT;
			newOffset += ofNoise(position * MID_SCALE  + MID_SPEED  * time) * mid.volume  / mid.maxVolume  * MID_HEIGHT;

			calculateNewPosForIndex(index, newOffset);
		}
	}
}

void Terrain::calculateNewPosForIndex(int index, float newHeight)
{
	//Change the x, y, and z values of the base vert over time
	ofVec3f randomVec = ofVec3f(ofRandomf() / 5, ofRandomf(), ofRandomf() / 5).normalize();
	ofVec3f basePos = baseMesh.getVertex(index);
	basePos += (randomVec * ofGetLastFrameTime() * randomPosScalar) + (basePos.y / 30000);
	//baseMesh.setVertex(index, newBasePos);

	//Change the depth in the direction of the normal vector
	ofVec3f normalVec = mesh.getNormal(index).normalize();
	ofVec3f newPos = basePos + newHeight * normalVec;

	//Set the new position to that of the vertex
	mesh.setVertex(index, newPos);
}
