#pragma include "../_material.frag"
#pragma include "../util.glsl"

uniform float edgeDistance;
uniform int mode;

in vec4 vWorldPosition;
in vec4 vColor;

const float FadeWidth = 0.5; // multiple of edgeDistance
const float WireframeWidth = 0.03;
const float ElevationWidth = 0.15;

void main()
{
	// Fade out over range (edgeDistance - FadeWidth)..edgeDistance using distance from center
	float a = length(vWorldPosition.xz);
	a /= edgeDistance;
	a = 1 - a;
	a /= FadeWidth;
	a = clamp01(a);

	if (a == 0) discard;

	vec3 color;
	if (mode == 0)
	{
		// Color from diffuse
		color = mat_diffuse.rgb + vec3(0.1) * vWorldPosition.y;

		// Hue from diffuse and sat/val from the vertex
		// color = mat_diffuse.rgb;
		// color = rgb2hsv(color);
		// color.yz = vColor.yz; // copy the vertex sat/val to the color
		// color.z += 0.2 * vWorldPosition.y; // increase value with y height
		// color = hsv2rgb(color);
	}
	else
	{
		// LSD RAINBOW!!! 🌈
		color = vec3(vWorldPosition.y * 0.1, 1, 1);
		color = hsv2rgb(color);
	}

	// Add topographic map lines
	if (fract(vColor.x) > 0.01 && fract(vColor.x) < ElevationWidth)
	{
		color = mix(color, vec3(1), 0.4);
	}

	// Inverse color on wireframe
//	if (outtexcoord.x < WireframeWidth || outtexcoord.x > 1 - WireframeWidth ||
//		outtexcoord.y < WireframeWidth || outtexcoord.y > 1 - WireframeWidth)
////		abs(outtexcoord.x - outtexcoord.y) < WireframeWidth
//	{
//		color = vec3(1) - color;
//	}

	vec4 zero = vec4(0);
	MATERIAL_FRAG_MAIN(zero, zero, vec4(color, a), zero);
}
