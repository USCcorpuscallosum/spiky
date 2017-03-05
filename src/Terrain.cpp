#include "Terrain.h"
#include "ofMain.h"

Terrain::Terrain()
{
	mesh.setMode(OF_PRIMITIVE_POINTS);
	
	length = 100;
	width = 100;
	skip = 5;
}

void Terrain::setSound(MusicAnalysis* ma)
{
	musicAnalysis = ma;
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
	//TODO: Implement this function
	int volume = musicAnalysis->getVolume();

}

//Height changes based on frequency
void Terrain::changeHeight()
{
	//Set all vertices based on pitch
}

void Terrain::draw()
{
	mesh.draw();
}

ofColor Terrain::getColor()
{
	return color;
}