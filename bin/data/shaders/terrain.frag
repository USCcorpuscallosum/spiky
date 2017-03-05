#version 150

in vec4 worldPosition;

out vec4 outputColor;
 
void main()
{
	outputColor = mix(vec4(0.48, 0, 0.91, 1), vec4(0.9, 0.05, 0.48, 1), worldPosition.z / 2);
}
