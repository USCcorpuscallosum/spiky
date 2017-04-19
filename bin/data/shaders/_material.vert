OUT vec2 outtexcoord; // pass the texCoord if needed
OUT vec3 transformedNormal;
OUT vec3 eyePosition3;

IN vec4 position;
IN vec4 color;
IN vec4 normal;
IN vec2 texcoord;

uniform float global_time;

// these are passed in from OF programmable renderer
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;

#define MATERIAL_VERT_MAIN(position, normal, texcoord) \
	vec4 eyePosition = modelViewMatrix * position; \
	vec3 tempNormal = (normalMatrix * normal).xyz; \
	transformedNormal = normalize(tempNormal); \
	eyePosition3 = (eyePosition.xyz) / eyePosition.w; \
	\
	outtexcoord = (textureMatrix*vec4(texcoord.x,texcoord.y,0,1)).xy; \
	gl_Position = modelViewProjectionMatrix * position;

#define MATERIAL_VERT_MAIN_DEFAULT() MATERIAL_VERT_MAIN(position, normal, texcoord)
