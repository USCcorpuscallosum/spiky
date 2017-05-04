#version 150
#pragma include "../util.glsl"

uniform int mode;
uniform vec4 color;
uniform float time;
uniform float volume;
uniform float aspectRatio;

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
	vec2 scaledTexcoord = outTexcoord;
	scaledTexcoord.x *= aspectRatio;
	scaledTexcoord.x -= (aspectRatio - 1) * 0.5;

	if (mode == 0)
	{
		// Night sky
		outputColor = vec4(0, 0.16, 0.43, 1.0);

		// Add vertical linear gradient from 0.5 to 1
		outputColor.rgb += GradientColorAdd * max(0, (scaledTexcoord.y - 0.5) * 2);
	}
	else
	{
		// Get triadic colors
		vec3 color = rgb2hsv(color.rgb);
		color.x += 0.333;

		float t = distance(scaledTexcoord, vec2(0.5, 0.5)) * 2; // 0..1 from center to edge
		t *= DistanceMult; // less emphasis on distance
		t += noise(scaledTexcoord * NoiseSize + time * NoiseSpeed) * NoiseMult; // add random noise which moves
		t += volume * VolumeMult;

		t = int(t * PosterizeSteps) / float(PosterizeSteps); // posterize

		color.x += t * 0.333;  // move forward in hues
		color.x = fract(color.x);
		color = hsv2rgb(color);

		outputColor = vec4(color, 1.0);
	}
}
