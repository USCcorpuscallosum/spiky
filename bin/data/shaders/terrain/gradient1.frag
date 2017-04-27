#pragma include "../_material.frag"
#pragma include "../util.glsl"

uniform float edgeDistance;

in vec4 vWorldPosition;
in vec4 vColor;

void main()
{
	const float fadeWidth = 0.5; // multiple of edgeDistance
	const float wireframeWidth = 0.03;

	// Fade out over range (edgeDistance - fadeWidth)..edgeDistance using distance from center
	float a = length(vWorldPosition.xz);
	a /= edgeDistance;
	a = 1 - a;
	a /= fadeWidth;
	a = clamp01(a);

	if (a == 0) discard;

	// Color from diffuse
	vec3 color = mat_diffuse.rgb + vec3(0.1) * vWorldPosition.y;

	// Hue from diffuse and sat/val from the vertex
//	vec3 color = mat_diffuse.rgb;
//	color = rgb2hsv(color);
//	color.yz = vColor.yz; // copy the vertex sat/val to the color
//	color.z += 0.2 * vWorldPosition.y; // increase value with y height
//	color = hsv2rgb(color);

	// LSD RAINBOW!!! 🌈
//	vec3 color = vec3(vWorldPosition.y * 0.1, 1, 1);
//	color = hsv2rgb(color);

	// Inverse color on wireframe
//	if (outtexcoord.x < wireframeWidth || outtexcoord.x > 1 - wireframeWidth ||
//		outtexcoord.y < wireframeWidth || outtexcoord.y > 1 - wireframeWidth)
////		abs(outtexcoord.x - outtexcoord.y) < wireframeWidth
//	{
//		color = vec3(1) - color;
//	}

	vec4 zero = vec4(0);
	MATERIAL_FRAG_MAIN(zero, zero, vec4(color, a), zero);
}
