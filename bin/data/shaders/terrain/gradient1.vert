#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;

out vec4 worldPosition;

void main()
{
	gl_Position = modelViewProjectionMatrix * position;
	worldPosition = position;
}
