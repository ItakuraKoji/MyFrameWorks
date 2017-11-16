#pragma once
#include<fstream>
#include<iostream>
#include<GLEW\glew.h>
#include<Eigen\Core>

using namespace Eigen;
class ShaderClass {
public:
	ShaderClass();
	~ShaderClass();

	bool Initialize(const char* vsFilename, const char* fsFilename);

	void SetShader();
	bool SetShaderParameter(float *world, float *view, float *projection);
	bool SetShaderTexture(GLuint textureLayer, GLuint textureNumber);
	bool SetShaderTexture2(GLuint textureLayer, GLuint textureNumber);
	bool SetShaderBoneMatrix(float* boneMat);
	bool SetShaderNumBone(int numBone);
	bool SetSkinningFunc(int in);
	bool SetLightParameter(const Vector3f& direction, const Vector4f& color, const float power);
	bool SetAmbientParameter(const Vector4f& color, const float power);

private:
	char* LoadTxtResource(const char* filename);
	void ShowShaderErrors(GLuint shaderID);
	void ShowLinkErrors(GLuint shaderID);

	GLuint shaderProgram;
	GLuint vertexShader;
	GLuint fragmentShader;
};
