#version 150

uniform vec4 terrainColor;
uniform float time;
uniform float volume;

in vec2 outTexcoord;

out vec4 outputColor;

// GLSL Noise from https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}
float noise(vec2 p){
	vec2 ip = floor(p);
	vec2 u = fract(p);
	u = u*u*(3.0-2.0*u);

	float res = mix(
		mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
		mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
	return res*res;
}

// RGB <-> HSV conversion from http://stackoverflow.com/a/17897228/1248884
vec3 rgb2hsv(vec3 c)
{
	vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
	vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
	vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

	float d = q.x - min(q.w, q.y);
	float e = 1.0e-10;
	return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}
vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

const float DistanceMult = 0.7;
const float NoiseSize = 20;
const vec2 NoiseSpeed = vec2(0, -10);
const float NoiseMult = 0.2;
const float VolumeMult = 0.5;
const int PosterizeSteps = 6;

void main()
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

	t = int(t * PosterizeSteps) / PosterizeSteps; // posterize

	color = mix(color, color2, t); // lerp between the colors

	outputColor = vec4(color, 0.04); // use alpha for blending
}
