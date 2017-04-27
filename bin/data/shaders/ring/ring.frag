#pragma include "../_material.frag"
#pragma include "../util.glsl"

uniform sampler2DRect spectrum;
uniform vec2 ringSize;
uniform vec2 scrollDirection;
uniform float fadeWidth;

in vec4 vWorldPosition;
in float vFrequency;

const float BaseAlpha = 0.2;
const float Brightness = 1.5;
// const vec2 ringSize = vec2(10, 20);
// const vec2 scrollDirection = vec2(0.5, -3.0); // sideways, inward
// const float fadeWidth = 0.2;

void main()
{

	float vol = getVolume(vFrequency, spectrum);

	vec2 mirroredUV = outtexcoord;
	mirroredUV.x = min(mirroredUV.x, 1.0 - mirroredUV.x); // mirror x over 0.5

	float t = noise(mirroredUV * ringSize + scrollDirection * global_time);
	t *= Brightness;

	vec3 emissiveColor = rgb2hsv(mat_diffuse.rgb);
	emissiveColor.x = fract(emissiveColor.x + 0.333 * (1 - mirroredUV.y)); // go forward in the color spectrum outward on the ring
	emissiveColor = hsv2rgb(emissiveColor);

	vec4 emissive = vec4(emissiveColor, mix(BaseAlpha, 1.0, t));

	// Fade to edges
	emissive.a *= min(
		clamp01(mirroredUV.y / fadeWidth), // inner edge
		clamp01((1.0 - mirroredUV.y) / fadeWidth)); // outer edge

	vec4 diffuse = vec4(0); // make entirely emissive
	MATERIAL_FRAG_MAIN(diffuse, mat_specular, emissive, mat_ambient)
}
