#pragma include "../_material.frag"
#pragma include "../util.glsl"

uniform sampler2DRect spectrum;
uniform float colorHueRange;

in vec4 vWorldPosition;
in float vFrequency;

void main()
{
	const float wireframeWidth = 0.03;
//	const float colorHueRange = 0.05;

	float vol = getVolume(vFrequency, spectrum);

	// Wireframe
//	if (outtexcoord.x < wireframeWidth || outtexcoord.x > 1 - wireframeWidth ||
//		outtexcoord.y < wireframeWidth || outtexcoord.y > 1 - wireframeWidth)
////		abs(outtexcoord.x - outtexcoord.y) < wireframeWidth
//	{
//		vol = 1;
//	}

	vec3 secondary = rgb2hsv(mat_diffuse.rgb);
	secondary.x = fract(secondary.x + log(vol) * colorHueRange); // go forward on the color spectrum with the volume
//	secondary.x = fract(secondary.x + vol * 0.333);
	secondary = hsv2rgb(secondary);

	vec4 emissive = vec4(secondary, 1);
	MATERIAL_FRAG_MAIN(mat_diffuse, mat_specular, emissive, mat_ambient)
}
