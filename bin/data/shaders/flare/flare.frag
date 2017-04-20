#pragma include "../_material.frag"
#pragma include "../util.glsl"

uniform float volume;
uniform float centerRadius;

const float DistanceMult = 0.7;
const vec2 NoiseSize = vec2(6.0, 4.0);
const vec2 NoiseSpeed = vec2(0, 6);
const float NoiseMult = 0.25;
const float VolumeFactor = 2.0;
const int PosterizeSteps = 5;

void main()
{
	// Get triadic colors
	vec3 color = rgb2hsv(mat_diffuse.rgb);
	color.x = fract(color.x + 0.333);
	color = hsv2rgb(color);
//	color = vec3(0,0,0);

	vec2 pos = (outtexcoord - vec2(0.5, 0.5)) * 2.0; // scale to -1..1 square
	float t = (length(pos) - centerRadius) / (1.0 - centerRadius); // 0..1 over centerRadius..1.0
//	t = max(0, t);
	t += 1.0 / PosterizeSteps; // move inward so that first step is on centerRadius
	t = 1 - t; // reverse so 1..0

	// Make a polar position
	vec2 arcpos = vec2(atan(abs(pos.y), pos.x), t); // abs(y) to prevent ugly seam

	// Add random noise which scrolls
	t -= noise(arcpos * NoiseSize + global_time * NoiseSpeed) * NoiseMult;

	// Make volume pull the value outward on a poly curve. Graph it
	t = 1 - pow(1 - t, 1 + volume * VolumeFactor);

	t = int(t * PosterizeSteps) / float(PosterizeSteps); // posterize

	vec4 outputColor = vec4(color, mat_diffuse.a * clamp01(t));

	vec4 zero = vec4(0);
	MATERIAL_FRAG_MAIN(zero, zero, outputColor, zero) // make entirely emissive
}
