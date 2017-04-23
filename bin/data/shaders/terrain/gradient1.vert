#pragma include "../_material.vert"

out vec4 vWorldPosition;
out vec4 vColor;

void main()
{
	vWorldPosition = position;
	vColor = color;

	MATERIAL_VERT_MAIN_DEFAULT()
}
