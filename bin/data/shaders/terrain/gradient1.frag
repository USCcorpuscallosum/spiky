#version 150

uniform vec4 primaryColor;
//uniform vec4 secondaryColor;

in vec4 worldPosition;

out vec4 outputColor;
 
void main()
{
	//if (int(worldPosition.z / 0.25) % 2 == 0)
	//	discard;

	outputColor = primaryColor + vec4(0.1, 0.1, 0.1, 0) * worldPosition.y;
}
