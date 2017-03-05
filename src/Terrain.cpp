#include "Terrain.h"
#include "ofMain.h"

Terrain::Terrain()
{
	length = 100;
	width = 100;
	skip = 5;

	color = ofColor_<float>(0.48, 0, 0.91);

	shaderNames.push_back("shaders/terrain/gradient1");

	for (int i = 0; i < shaderNames.size(); i++)
	{
		shaders.push_back(ofShader());
		shaders[i].load(shaderNames[i]);
	}
	activeShader = 0;
}

Terrain::Terrain(float init_length, float init_width)
{
	length = init_length;
	width = init_width;
}

Terrain::Terrain(float init_length, float init_width, float init_skip)
{
	length = init_length;
	width = init_width;
	skip = init_skip;
}


//Sets up all the vertices for the mesh
void Terrain::initializeTerrain()
{
	//Create vertices
	for (int y = 0; y < length; y++) 
	{
		for (int x = 0; x<width; x++) 
		{
			mesh.addVertex(ofPoint(x, y, 0));	// mesh index = x + y*width
												// this replicates the pixel array within the camera bitmap...
			mesh.addColor(ofFloatColor(0, 0, 0));  // placeholder for colour data, we'll get this from the camera
		}
	}

	//Create indeces
	for (int y = 0; y < length - 1; y++)
	{
		for (int x = 0; x < width - 1; x++)
		{
			mesh.addIndex(x + y*width);              
			mesh.addIndex((x + 1) + y*width);        
			mesh.addIndex(x + (y + 1)*width);        

			mesh.addIndex((x + 1) + y*width);        
			mesh.addIndex((x + 1) + (y + 1)*width);  
			mesh.addIndex(x + (y + 1)*width);        
		}
	}
}

//Color changes based on volume
//Give each index its specific color
void Terrain::changeAllColors()
{
	// Cycle colors
	float hue, saturation, brightness;
	color.getHsb(hue, saturation, brightness);
	hue += ofGetLastFrameTime() * CYCLE_SPEED;
	if (hue >= 256) hue -= 256;
	color = ofColor::fromHsb(hue, saturation, brightness);
}

//Height changes based on frequency
void Terrain::changeHeight()
{
	const float BASS_HEIGHT = 9.f;
	const float BASS_SCALE = 0.05f; // smaller means larger regions
	const ofVec2f BASS_SPEED(0.8f, 0.2f);
	const float MID_HEIGHT = 4.f;
	const float MID_SCALE = 0.4f;
	const ofVec2f MID_SPEED(0.2f, -0.5f);

	float time = ofGetElapsedTimef();

	float bassMax;
	float bass = musicAnalysis->getVolumeOfRange(0, 200, &bassMax);

	float midMax;
	float mid = musicAnalysis->getVolumeOfRange(300, 600, &midMax);

	for (int y = 0; y < length; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int index = y * length + x;
			ofVec2f position(x, y);

			// Calculate the new height of each vertex from the audio
			float height = 0;
			height += ofNoise(position * BASS_SCALE + BASS_SPEED * time) * bass / bassMax * BASS_HEIGHT;
			height += ofNoise(position * MID_SCALE  + MID_SPEED  * time) * mid  / midMax  * MID_HEIGHT;

			mesh.setVertex(index, ofVec3f(x, y, height));
		}
	}
}

void Terrain::draw()
{
	shaders[activeShader].begin();
	shaders[activeShader].setUniform4f("primaryColor", color.r / 256.f, color.g / 256.f, color.b / 256.f, 1);
	//ofColor secondaryColor = color + ofColor(50, 50, 50);
	//shaders[activeShader].setUniform4f("secondaryColor", secondaryColor.r / 256.f, secondaryColor.g / 256.f, secondaryColor.b / 256.f, 1);

	mesh.draw();
	shaders[activeShader].end();
}
