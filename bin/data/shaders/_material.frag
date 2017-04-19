IN vec2 outtexcoord; // pass the texCoord if needed
IN vec3 transformedNormal;
// Eye-coordinate position of vertex
IN vec3 eyePosition3;


struct lightData
{
	float enabled;
	vec4 ambient;
	float type; // 0 = pointlight 1 = directionlight
	vec4 position; // where are we
	vec4 diffuse; // how diffuse
	vec4 specular; // what kinda specular stuff we got going on?
	// attenuation
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	// only for spot
	float spotCutoff;
	float spotCosCutoff;
	float spotExponent;
	// spot and area
	vec3 spotDirection;
	// only for directional
	vec3 halfVector;
	// only for area
	float width;
	float height;
	vec3 right;
	vec3 up;
};

uniform SAMPLER tex0;

uniform vec4 mat_ambient;
uniform vec4 mat_diffuse;
uniform vec4 mat_specular;
uniform vec4 mat_emissive;
uniform float mat_shininess;
uniform vec4 global_ambient;
uniform float global_time;

// these are passed in from OF programmable renderer
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

uniform lightData lights[MAX_LIGHTS];


void pointLight( in lightData light, in vec3 normal, in vec3 ecPosition3, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular ){
	float nDotVP;       // normal . light direction
	float nDotHV;       // normal . light half vector
	float pf;           // power factor
	float attenuation;  // computed attenuation factor
	float d;            // distance from surface to light source
	vec3  VP;           // direction from surface to light position
	vec3  halfVector;   // direction of maximum highlights
	vec3 eye = vec3 (0.0, 0.0, 1.0);

	// Compute vector from surface to light position
	VP = vec3 (light.position.xyz) - ecPosition3;

	// Compute distance between surface and light position
	d = length(VP);


	// Compute attenuation
	attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);

	// Normalize the vector from surface to light position
	VP = normalize(VP);
	halfVector = normalize(VP + eye);
	nDotHV = max(0.0, dot(normal, halfVector));
	nDotVP = max(0.0, dot(normal, VP));


	ambient += light.ambient.rgb * attenuation;
	diffuse += light.diffuse.rgb * nDotVP * attenuation;
#ifndef TARGET_OPENGLES
#define SPECULAR_REFLECTION
#endif
#ifndef SPECULAR_REFLECTION
	// ha! no branching :)
	pf = mix(0.0, pow(nDotHV, mat_shininess), step(0.0000001, nDotVP));
	specular += light.specular.rgb * pf * nDotVP * attenuation;
#else
	// fresnel factor
	// http://en.wikibooks.org/wiki/GLSL_Programming/Unity/Specular_Highlights_at_Silhouettes
	float w = pow(1.0 - max(0.0, dot(halfVector, VP)), 5.0);
	vec3 specularReflection = attenuation * vec3(light.specular.rgb)
	  * mix(vec3(mat_specular.rgb), vec3(1.0), w)
	  * pow(nDotHV, mat_shininess);
	specular += mix(vec3(0.0), specularReflection, step(0.0000001, nDotVP));
#endif
}

void directionalLight(in lightData light, in vec3 normal, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
	float nDotVP;         // normal . light direction
	float nDotHV;         // normal . light half vector
	float pf;             // power factor

	nDotVP = max(0.0, dot(normal, normalize(vec3(light.position))));
	nDotHV = max(0.0, dot(normal, light.halfVector));

	pf = mix(0.0, pow(nDotHV, mat_shininess), step(0.0000001, nDotVP));

	ambient += light.ambient.rgb;
	diffuse += light.diffuse.rgb * nDotVP;
	specular += light.specular.rgb * pf * nDotVP;
}

void spotLight(in lightData light, in vec3 normal, in vec3 ecPosition3, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
	float nDotVP; // = max(dot(normal,normalize(vec3(light.position))),0.0);
	float nDotHV;       // normal . light half vector
	float pf;
	float d;            // distance from surface to light source
	vec3  VP;           // direction from surface to light position
	vec3 eye = vec3 (0.0, 0.0, 1.0);
	float spotEffect;
	float attenuation=1.0;
	vec3  halfVector;   // direction of maximum highlights
	// Compute vector from surface to light position
	VP = light.position.xyz - ecPosition3;
	spotEffect = dot(light.spotDirection, -normalize(VP));

	if (spotEffect > light.spotCosCutoff) {
		// Compute distance between surface and light position
		d = length(VP);
		spotEffect = pow(spotEffect, light.spotExponent);
		attenuation = spotEffect / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);

		VP = normalize(VP);
		halfVector = normalize(VP + eye);
		nDotHV = max(0.0, dot(normal, halfVector));
		nDotVP = max(0.0, dot(normal, VP));

		pf = mix(0.0, pow(nDotHV, mat_shininess), step(0.0000001, nDotVP));

		diffuse += light.diffuse.rgb * nDotVP * attenuation;
		specular += light.specular.rgb * pf * nDotVP * attenuation;

	}

	ambient += light.ambient.rgb * attenuation;

}


vec3 projectOnPlane(in vec3 point, in vec3 planeCenter, in vec3 planeNormal){
	return point - dot( point - planeCenter, planeNormal ) * planeNormal;
}

vec3 linePlaneIntersect(in vec3 lp, in vec3 lv, in vec3 pc, in vec3 pn){
   return lp+lv*(dot(pn,pc-lp)/dot(pn,lv));
}

void areaLight(in lightData light, in vec3 N, in vec3 V, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
	vec3 right = light.right;
	vec3 pnormal = light.spotDirection;
	vec3 up = light.up;

	//width and height of the area light:
	float width = light.width*0.5;
	float height = light.height*0.5;
	float attenuation;

	//project onto plane and calculate direction from center to the projection.
	vec3 projection = projectOnPlane(V,light.position.xyz,pnormal);// projection in plane
	vec3 dir = projection-light.position.xyz;

	//calculate distance from area:
	vec2 diagonal = vec2(dot(dir,right),dot(dir,up));
	vec2 nearest2D = vec2(clamp( diagonal.x,-width,width  ),clamp(  diagonal.y,-height,height));
	vec3 nearestPointInside = vec3(light.position.xyz)+(right*nearest2D.x+up*nearest2D.y);
	float dist = distance(V,nearestPointInside);//real distance to area rectangle

	vec3 lightDir = normalize(nearestPointInside - V);
	attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * dist + light.quadraticAttenuation * dist * dist);

	float NdotL = max( dot( pnormal, -lightDir ), 0.0 );
	float NdotL2 = max( dot( N, lightDir ), 0.0 );
	if ( NdotL * NdotL2 > 0.0 ) {
		float diffuseFactor = sqrt( NdotL * NdotL2 );
		vec3 R = reflect( normalize( -V ), N );
		vec3 E = linePlaneIntersect( V, R, light.position.xyz, pnormal );
		float specAngle = dot( R, pnormal );
		if (specAngle > 0.0){
			vec3 dirSpec = E - light.position.xyz;
			vec2 dirSpec2D = vec2(dot(dirSpec,right),dot(dirSpec,up));
			vec2 nearestSpec2D = vec2(clamp( dirSpec2D.x,-width,width  ),clamp(  dirSpec2D.y,-height,height));
			float specFactor = 1.0-clamp(length(nearestSpec2D-dirSpec2D) * 0.05 * mat_shininess,0.0,1.0);
			specular += light.specular.rgb * specFactor * specAngle * diffuseFactor * attenuation;
		}
		diffuse  += light.diffuse.rgb  * diffuseFactor * attenuation;
	}
	ambient += light.ambient.rgb * attenuation;
}




//////////////////////////////////////////////////////
// here's the main method
//////////////////////////////////////////////////////

#define MATERIAL_FRAG_MAIN(diffuseColor, specularColor, emissiveColor, ambientColor) \
	vec3 ambient_ = global_ambient.rgb; \
	vec3 diffuse_ = vec3(0.0,0.0,0.0); \
	vec3 specular_ = vec3(0.0,0.0,0.0); \
	\
	for( int i = 0; i < MAX_LIGHTS; i++ ){ \
		if(lights[i].enabled<0.5) continue; \
		if(lights[i].type<0.5){ \
			pointLight(lights[i], transformedNormal, eyePosition3, ambient_, diffuse_, specular_); \
		}else if(lights[i].type<1.5){ \
			directionalLight(lights[i], transformedNormal, ambient_, diffuse_, specular_); \
		}else if(lights[i].type<2.5){ \
			spotLight(lights[i], transformedNormal, eyePosition3, ambient_, diffuse_, specular_); \
		}else{ \
			areaLight(lights[i], transformedNormal, eyePosition3, ambient_, diffuse_, specular_); \
		} \
	} \
	\
	vec4 localColor = vec4(ambient_ * ambientColor.rgb + diffuse_ * diffuseColor.rgb + specular_ * specularColor.rgb, diffuseColor.a) + emissiveColor; \
	FRAG_COLOR = clamp( localColor, 0.0, 1.0 );

#define MATERIAL_FRAG_MAIN_DEFAULT() MATERIAL_FRAG_MAIN(mat_diffuse, mat_specular, mat_emissive, mat_ambient)

