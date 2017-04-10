#pragma include "../_material.frag"
#pragma include "../util.glsl"

uniform sampler2DRect spectrum;

in vec4 vWorldPosition;
in float vFrequency;

void main()
{
	const vec2 ringSize = vec2(10, 20);
	const vec2 scrollDirection = vec2(0.5, -4.0);
	const vec4 baseColor = vec4(0.1, 0.1, 1.0, 0.2);
	const vec4 ringColor = vec4(0.4, 0.2, 1.0, 1.0);
	const float fadeWidth = 0.2;
	const float brightness = 1.5;

	float vol = getVolume(vFrequency, spectrum);

	vec2 mirroredUV = outtexcoord;
	mirroredUV.x = min(mirroredUV.x, 1.0 - mirroredUV.x); // mirror x over 0.5

	float t = noise(mirroredUV * ringSize + scrollDirection * global_time);
	t *= brightness;

	vec4 emissive = mix(baseColor, ringColor, t);

	// Fade to edges
	emissive.a *= min(
		clamp01(mirroredUV.y / fadeWidth), // inner edge
		clamp01((1.0 - mirroredUV.y) / fadeWidth)); // outer edge

	vec4 diffuse = vec4(0); // make entirely emissive
	MATERIAL_FRAG_MAIN(diffuse, mat_specular, emissive, mat_ambient)
}
