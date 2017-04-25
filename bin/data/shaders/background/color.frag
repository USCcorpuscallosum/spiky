#version 150

uniform vec4 terrainColor;

in vec2 outTexcoord;

out vec4 outputColor;

void main()
{
	const vec3 gradientColorAdd = vec3(0.3, 0.3, 0.3);

	outputColor = vec4(terrainColor.rgb, 1.0);

	// Add vertical linear gradient from 0.5 to 1
	outputColor.rgb += gradientColorAdd * max(0, (outTexcoord.y - 0.5) * 2);
}
