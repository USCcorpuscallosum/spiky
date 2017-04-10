#include "ofCustomMaterial.h"
#include "ofConstants.h"
#include "ofGLUtils.h"
#include "ofLight.h"
#include "ofGLProgrammableRenderer.h"
#include "ofAppRunner.h"

ofCustomMaterial::Data::Data()
:diffuse(0.8f, 0.8f, 0.8f, 1.0f)
,ambient(0.2f, 0.2f, 0.2f, 1.0f)
,specular(0.0f, 0.0f, 0.0f, 1.0f)
,emissive(0.0f, 0.0f, 0.0f, 1.0f)
,shininess(0.2f){

}

ofCustomMaterial::ofCustomMaterial() {
}

void ofCustomMaterial::setColors(ofFloatColor oDiffuse, ofFloatColor oAmbient, ofFloatColor oSpecular, ofFloatColor oEmissive) {
	setDiffuseColor(oDiffuse);
	setAmbientColor(oAmbient);
	setSpecularColor(oSpecular);
	setEmissiveColor(oEmissive);
}

void ofCustomMaterial::setDiffuseColor(ofFloatColor oDiffuse) {
	data.diffuse = oDiffuse;
}

void ofCustomMaterial::setAmbientColor(ofFloatColor oAmbient) {
	data.ambient = oAmbient;
}

void ofCustomMaterial::setSpecularColor(ofFloatColor oSpecular) {
	data.specular = oSpecular;
}

void ofCustomMaterial::setEmissiveColor(ofFloatColor oEmissive) {
	data.emissive = oEmissive;
}

void ofCustomMaterial::setShininess(float nShininess) {
	data.shininess = nShininess;
}

void ofCustomMaterial::setData(const ofCustomMaterial::Data &data){
	this->data = data;
}

float ofCustomMaterial::getShininess()const{
	return data.shininess;
}

ofFloatColor ofCustomMaterial::getDiffuseColor()const {
	return data.diffuse;
}

ofFloatColor ofCustomMaterial::getAmbientColor()const {
	return data.ambient;
}

ofFloatColor ofCustomMaterial::getSpecularColor()const {
	return data.specular;
}

ofFloatColor ofCustomMaterial::getEmissiveColor()const {
	return data.emissive;
}

ofCustomMaterial::Data ofCustomMaterial::getData()const{
	return data;
}

void ofCustomMaterial::begin() const{
	if(ofGetGLRenderer()){
		ofGetGLRenderer()->bind(*this);
	}
}

void ofCustomMaterial::end() const{
	if(ofGetGLRenderer()){
		ofGetGLRenderer()->unbind(*this);
	}
}

void ofCustomMaterial::load(string shaderName) {
	load(shaderName + ".vert", shaderName + ".frag");
}

void ofCustomMaterial::load(string vertShaderPath, string fragShaderPath){
	ofBuffer buffer = ofBufferFromFile(vertShaderPath);
	vertexShader = buffer.getText();
	string absoluteFilePath = ofFilePath::getAbsolutePath(vertShaderPath, true);
	vertexShaderDirectoryPath = ofFilePath::getEnclosingDirectory(absoluteFilePath, false);
	if(buffer.size() == 0) {
		ofLogError("ofCustomMaterial") << "load(): couldn't load vert shader " << " from \"" << vertShaderPath << "\"";
		return;
	}

	buffer = ofBufferFromFile(fragShaderPath);
	fragmentShader = buffer.getText();
	absoluteFilePath = ofFilePath::getAbsolutePath(fragShaderPath, true);
	fragmentShaderDirectoryPath = ofFilePath::getEnclosingDirectory(absoluteFilePath,false);
	if(buffer.size() == 0) {
		ofLogError("ofCustomMaterial") << "load(): couldn't load frag shader " << " from \"" << fragShaderPath << "\"";
		return;
	}

	auto& renderer = *static_pointer_cast<ofGLProgrammableRenderer>(ofGetCurrentRenderer());
	initShaders(renderer);
}

void ofCustomMaterial::initShaders(ofGLProgrammableRenderer & renderer) {
	if(!shadersInitialized || ofLightsData().size()!=shaderLights){
#ifndef TARGET_OPENGLES
		string vertexRectHeader = renderer.defaultVertexShaderHeader(GL_TEXTURE_RECTANGLE);
		string fragmentRectHeader = renderer.defaultFragmentShaderHeader(GL_TEXTURE_RECTANGLE);
#endif
		string vertex2DHeader = renderer.defaultVertexShaderHeader(GL_TEXTURE_2D);
		string fragment2DHeader = renderer.defaultFragmentShaderHeader(GL_TEXTURE_2D);
		shaderLights = ofLightsData().size();
		shaderNoTexture.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertex2DHeader,shaderLights,false),vertexShaderDirectoryPath);
		shaderNoTexture.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragment2DHeader,shaderLights,false),fragmentShaderDirectoryPath);
		shaderNoTexture.bindDefaults();
		shaderNoTexture.linkProgram();

		shaderTexture2D.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertex2DHeader,shaderLights,true),vertexShaderDirectoryPath);
		shaderTexture2D.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragment2DHeader,shaderLights,true),fragmentShaderDirectoryPath);
		shaderTexture2D.bindDefaults();
		shaderTexture2D.linkProgram();

#ifndef TARGET_OPENGLES
		shaderTextureRect.setupShaderFromSource(GL_VERTEX_SHADER,vertexSource(vertexRectHeader,shaderLights,true),vertexShaderDirectoryPath);
		shaderTextureRect.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentSource(fragmentRectHeader,shaderLights,true),fragmentShaderDirectoryPath);
		shaderTextureRect.bindDefaults();
		shaderTextureRect.linkProgram();
#endif

		shadersInitialized = true;
	}
}

const ofShader& ofCustomMaterial::getShader(int textureTarget /*= GL_TEXTURE_2D*/) const{
	auto& renderer = *static_pointer_cast<ofGLProgrammableRenderer>(ofGetCurrentRenderer());
	return getShader(textureTarget, renderer);
}

const ofShader & ofCustomMaterial::getShader(int textureTarget, ofGLProgrammableRenderer & renderer) const{
	switch(textureTarget){
	case OF_NO_TEXTURE:
		return shaderNoTexture;
		break;
	case GL_TEXTURE_2D:
		return shaderTexture2D;
		break;
	default:
		return shaderTextureRect;
		break;
	}
}

void ofCustomMaterial::updateMaterial(const ofShader & shader,ofGLProgrammableRenderer & renderer) const{
	shader.setUniform4fv("mat_ambient", &data.ambient.r);
	shader.setUniform4fv("mat_diffuse", &data.diffuse.r);
	shader.setUniform4fv("mat_specular", &data.specular.r);
	shader.setUniform4fv("mat_emissive", &data.emissive.r);
	shader.setUniform4fv("global_ambient", &ofGetGlobalAmbientColor().r);
	shader.setUniform1f("mat_shininess",data.shininess);
	shader.setUniform1f("global_time", ofGetElapsedTimef());
}

void ofCustomMaterial::updateLights(const ofShader & shader,ofGLProgrammableRenderer & renderer) const{
	for(size_t i=0;i<ofLightsData().size();i++){
		string idx = ofToString(i);
		shared_ptr<ofLight::Data> light = ofLightsData()[i].lock();
		if(!light || !light->isEnabled){
			shader.setUniform1f("lights["+idx+"].enabled",0);
			continue;
		}
		ofVec4f lightEyePosition = light->position * renderer.getCurrentViewMatrix();
		shader.setUniform1f("lights["+idx+"].enabled",1);
		shader.setUniform1f("lights["+idx+"].type", light->lightType);
		shader.setUniform4f("lights["+idx+"].position", lightEyePosition);
		shader.setUniform4f("lights["+idx+"].ambient", light->ambientColor);
		shader.setUniform4f("lights["+idx+"].specular", light->specularColor);
		shader.setUniform4f("lights["+idx+"].diffuse", light->diffuseColor);

		if(light->lightType!=OF_LIGHT_DIRECTIONAL){
			shader.setUniform1f("lights["+idx+"].constantAttenuation", light->attenuation_constant);
			shader.setUniform1f("lights["+idx+"].linearAttenuation", light->attenuation_linear);
			shader.setUniform1f("lights["+idx+"].quadraticAttenuation", light->attenuation_quadratic);
		}

		if(light->lightType==OF_LIGHT_SPOT){
			ofVec3f direction = light->position + light->direction;
			direction = direction * renderer.getCurrentViewMatrix();
			direction = direction - lightEyePosition;
			shader.setUniform3f("lights["+idx+"].spotDirection", direction.normalize());
			shader.setUniform1f("lights["+idx+"].spotExponent", light->exponent);
			shader.setUniform1f("lights["+idx+"].spotCutoff", light->spotCutOff);
			shader.setUniform1f("lights["+idx+"].spotCosCutoff", cos(ofDegToRad(light->spotCutOff)));
		}else if(light->lightType==OF_LIGHT_DIRECTIONAL){
			ofVec3f halfVector = (ofVec3f(0,0,1) + lightEyePosition).getNormalized();
			shader.setUniform3f("lights["+idx+"].halfVector", halfVector);
		}else if(light->lightType==OF_LIGHT_AREA){
			shader.setUniform1f("lights["+idx+"].width", light->width);
			shader.setUniform1f("lights["+idx+"].height", light->height);
			ofVec3f direction = light->position + light->direction;
			direction = direction * renderer.getCurrentViewMatrix();
			direction = direction - lightEyePosition;
			shader.setUniform3f("lights["+idx+"].spotDirection", direction.normalize());
			ofVec3f right = light->position + light->right;
			right = right * renderer.getCurrentViewMatrix();
			right = right - lightEyePosition;
			ofVec3f up = right.getCrossed(direction);
			shader.setUniform3f("lights["+idx+"].right", right.normalize());
			shader.setUniform3f("lights["+idx+"].up", up.normalize());
		}
	}
}

static string shaderHeader(string header, int maxLights, bool hasTexture){
	header += "#define MAX_LIGHTS " + ofToString(max(1,maxLights)) + "\n";
	if(hasTexture){
		header += "#define HAS_TEXTURE\n";
	}
	return header;
}

string ofCustomMaterial::vertexSource(string defaultHeader, int maxLights, bool hasTexture) const{
	if (vertexShader.empty()){
		ofLogError("ofCustomMaterial") << "vertexSource(): vertex shader is empty";
	}
	return shaderHeader(defaultHeader,maxLights,hasTexture) + vertexShader;
}

string ofCustomMaterial::fragmentSource(string defaultHeader, int maxLights, bool hasTexture) const{
	if (fragmentShader.empty()){
		ofLogError("ofCustomMaterial") << "fragmentSource(): fragment shader is empty";
	}
	return shaderHeader(defaultHeader,maxLights,hasTexture) + fragmentShader;
}
