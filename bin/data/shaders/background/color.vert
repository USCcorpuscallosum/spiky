#version 150

in vec4 position;
in vec2 texcoord;

out vec2 outTexcoord;

void main()
{
	// Use the vertex positions directly for the full-screen quad
	gl_Position = position;

	outTexcoord = texcoord;
}
