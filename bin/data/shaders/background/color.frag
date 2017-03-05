#version 150

uniform vec4 terrainColor;

in vec2 outTexcoord;

out vec4 outputColor;
 
void main()
{
	// Invert the terrain color
	outputColor = vec4(1 - terrainColor.r, 1 - terrainColor.g, 1 - terrainColor.b, 1);
}
