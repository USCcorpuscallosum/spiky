#version 150
#pragma include "../util.glsl"

uniform vec4 terrainColor;
uniform int mode;
uniform float time;
uniform float volume;

in vec2 outTexcoord;

out vec4 outputColor;

const vec3 GradientColorAdd = vec3(0.3, 0.3, 0.3);
const float DistanceMult = 0.7;
const float NoiseSize = 20;
const vec2 NoiseSpeed = vec2(0, -10);
const float NoiseMult = 0.2;
const float VolumeMult = 0.5;
const int PosterizeSteps = 6;

void main()
{

	if (mode == 0)
	{
		// Night sky
		outputColor = vec4(0, 0.16, 0.43, 1.0);

		// Add vertical linear gradient from 0.5 to 1
		outputColor.rgb += GradientColorAdd * max(0, (outTexcoord.y - 0.5) * 2);
	}
	else
	{
		// Get triadic colors
		vec3 color = rgb2hsv(terrainColor.rgb);
		vec3 color2 = color;
		color.x = fract(color.x + 0.333);
		color2.x = fract(color.x + 0.333);
		color = hsv2rgb(color);
		color2 = hsv2rgb(color2);

		float t = distance(outTexcoord, vec2(0.5, 0.5)) * 2; // 0..1 from center to edge
		t *= DistanceMult; // less emphasis on distance
		t += noise(outTexcoord * NoiseSize + time * NoiseSpeed) * NoiseMult; // add random noise which moves
		t += volume * VolumeMult;

		t = int(t * PosterizeSteps) / float(PosterizeSteps); // posterize

		color = mix(color, color2, t); // lerp between the colors

		outputColor = vec4(color, 1.0);
	}
}
