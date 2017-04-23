#pragma include "../_material.frag"
#pragma include "../util.glsl"

in vec4 vWorldPosition;
in vec4 vColor;

void main()
{
	//if (int(worldPosition.z / 0.25) % 2 == 0)
	//	discard;

	vec3 color = mat_diffuse.rgb + vec3(0.1) * vWorldPosition.y;

	vec4 zero = vec4(0);
	MATERIAL_FRAG_MAIN(zero, zero, vec4(color, 1.0), zero);
}
