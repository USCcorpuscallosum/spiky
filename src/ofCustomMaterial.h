#pragma once
#include "ofColor.h"
#include "ofShader.h"
#include "ofBaseTypes.h"

/**
 * Material concept: "Anything graphical applied to the polygons"
 * 
 * @author	Diederick Huijbers <diederick[at]apollomedia[dot]nl> 
 *
 * @link	Wavefront material file spec:
 *			http://people.sc.fsu.edu/~jburkardt/data/mtl/mtl.html
 * @link	Ogre3D
 *			http://www.ogre3d.org/docs/manual/manual_11.html#SEC14
 * @link	assim material
 *			http://assimp.sourceforge.net/lib_html/ai_material_8h.html#7dd415ff703a2cc53d1c22ddbbd7dde0
 */


class ofCustomMaterial: public ofBaseMaterial {
public:
	ofCustomMaterial();
	virtual ~ofCustomMaterial(){};
	
	// set colors
	void setColors(ofFloatColor oDiffuse, ofFloatColor oAmbient, ofFloatColor oSpecular, ofFloatColor emissive);
	void setDiffuseColor(ofFloatColor oDiffuse);
	void setAmbientColor(ofFloatColor oAmbient);
	void setSpecularColor(ofFloatColor oSpecular);
	void setEmissiveColor(ofFloatColor oEmmisive);
	void setShininess(float nShininess);

	ofFloatColor getDiffuseColor() const;
	ofFloatColor getAmbientColor() const;
	ofFloatColor getSpecularColor() const;
	ofFloatColor getEmissiveColor() const;
	float getShininess() const;
	
	struct Data{
		Data();
		ofFloatColor diffuse;
		ofFloatColor ambient;
		ofFloatColor specular;
		ofFloatColor emissive;
		float shininess;
	};
	Data getData() const;
	void setData(const ofCustomMaterial::Data& data);
	
	// apply the material
	void begin() const;
	void end() const;

	void load(string shaderName);
	void load(string vertShaderPath, string fragShaderPath);
	const ofShader& getShader(int textureTarget = GL_TEXTURE_2D) const;
	const ofShader & getShader(int textureTarget, ofGLProgrammableRenderer & renderer) const;

private:
	void initShaders(ofGLProgrammableRenderer & renderer);
	void updateMaterial(const ofShader & shader,ofGLProgrammableRenderer & renderer) const;
	void updateLights(const ofShader & shader,ofGLProgrammableRenderer & renderer) const;

	string vertexSource(string defaultHeader, int maxLights, bool hasTexture) const;
	string fragmentSource(string defaultHeader, int maxLights, bool hasTexture) const;
	
	Data data;

	ofShader shaderNoTexture;
	ofShader shaderTexture2D;
	ofShader shaderTextureRect;
	bool shadersInitialized = false;
	size_t shaderLights = 0;
	string vertexShader;
	string vertexShaderDirectoryPath;
	string fragmentShader;
	string fragmentShaderDirectoryPath;
};
