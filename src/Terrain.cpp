#include "Terrain.h"
#include "ofMain.h"

Terrain::Terrain(MusicAnalysis*)
{
	length = 400;
	width = 400;
	skip = 5;
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
	for (int y = 0; y < length; y++)
	{
		for (int x = 0; x < width; x++)
		{
			mesh.addIndex(x + y*width);
			mesh.addIndex((x + 1) + y*width);
			mesh.addIndex((x + 1) + (y + 1)*width);
			mesh.addIndex(x + (y + 1)*width);
			mesh.addIndex(x + y*width);
		}
	}
}

//Color changes based on volume
//Give each index its specific color
void Terrain::changeColor()
{
	//One line, changes one color variable
}

//Height changes based on frequency
void Terrain::changeHeihgt()
{
	//Set all vertices based on pitch
}

void Terrain::draw()
{
	//Just call the draw function
}

ofColor Terrain::getColor()
{
	return color;
}