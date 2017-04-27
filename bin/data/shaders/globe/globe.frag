#pragma include "../_material.frag"
#pragma include "../util.glsl"

uniform sampler2DRect spectrum;
uniform float colorHueRange;

in vec4 vWorldPosition;
in float vFrequency;

const float WireframeWidth = 0.03;

void main()
{
	float vol = getVolume(vFrequency, spectrum);

	// Wireframe
//	if (outtexcoord.x < WireframeWidth || outtexcoord.x > 1 - WireframeWidth ||
//		outtexcoord.y < WireframeWidth || outtexcoord.y > 1 - WireframeWidth)
////		abs(outtexcoord.x - outtexcoord.y) < WireframeWidth
//	{
//		vol = 1;
//	}

	vec3 secondary = rgb2hsv(mat_diffuse.rgb);
	secondary.x = fract(secondary.x + log(vol + 0.001) * colorHueRange); // go forward on the color spectrum with the volume
//	secondary.x = fract(secondary.x + vol * 0.333);
	secondary = hsv2rgb(secondary);

	vec4 emissive = vec4(secondary, 1);
	MATERIAL_FRAG_MAIN(mat_diffuse, mat_specular, emissive, mat_ambient)
}
