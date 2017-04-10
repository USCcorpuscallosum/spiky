#pragma include "../_material.vert"
#pragma include "../util.glsl"

uniform sampler2DRect spectrum;
uniform float amplitude;
uniform int bins;

out vec4 vWorldPosition;
out float vFrequency;

void main()
{
	// Get the volume from the texture and offset the position by the volume
	vFrequency = texcoord.x * bins;
	vFrequency += 0.5; // the first pixel is duplicated at the end, so transition between the first and last
	float vol = getVolume(vFrequency, spectrum);

	vec4 newPosition = position;

	// (1) Move up based on volume
	vol *= (int(vFrequency) % bins % 2) * 2 - 1; // invert vol if on odd bin
	newPosition.xyz += normal.xyz * vol * amplitude;

	// (2) Expand outward based on volume
//	newPosition.xyz += texcoord.y * normalize(position).xyz * vol * amplitude;

	vWorldPosition = newPosition;
	MATERIAL_VERT_MAIN(newPosition, normal, texcoord)
}
