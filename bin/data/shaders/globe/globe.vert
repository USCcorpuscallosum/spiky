#pragma include "../_material.vert"
#pragma include "../util.glsl"

uniform sampler2DRect spectrum;
uniform float amplitude;
uniform int bins;
in float frequency;

out vec4 vWorldPosition;
out float vFrequency;

void main()
{
	// Get the volume from the texture and offset the position by the volume
	float vol = getVolumeBins(frequency, bins, spectrum);
	vec4 sphereNormal = vec4(position.xyz, 0); // homogeneous
	sphereNormal = normalize(sphereNormal);

	vec4 newPosition = position + sphereNormal * vol * amplitude;
	vWorldPosition = newPosition;
	vFrequency = frequency;

	MATERIAL_VERT_MAIN(newPosition, normal, texcoord)
}
