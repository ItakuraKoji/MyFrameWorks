#pragma once
#include<fstream>
#include<iostream>
#include<GLEW\glew.h>
#include<Eigen\Core>

using namespace Eigen;

//シェーダーの管理とuniform変数の受け渡しを担当するクラス
//uniform変数名は固定的な機能を除くと、関数化にキリがないので汎用的な関数を用意
class ShaderClass {
public:
	ShaderClass();
	~ShaderClass();

	bool Initialize(std::string vsFilename, std::string fsFilename);

	void UseShader();
	void SetMatrix          (Matrix4f& mat);
	void SetWorldMatrix     (Matrix4f& world);
	void SetTexture         (const char* uniformName, GLuint textureLayer, GLuint textureNumber);
	void SetDirectionalLight(float power, Vector4f& color, Vector3f& direction);
	void SetAmbientLight    (float power, Vector4f& color);

	//汎用的なuniform変数への受け渡し
	void SetValue(const char* uniformName, int value);
	void SetValue(const char* uniformName, float value);
	void SetValue(const char* uniformName, Matrix4f& value);
	void SetValue(const char* uniformName, Vector4f& value);
	void SetValue(const char* uniformName, Vector3f& value);

	//subroutineの設定(vertex shader)
	void SetVertexShaderSubroutine(const char* subroutineFunctionName);
	//subroutineの設定(fragment shader)
	void SetFragmentShaderSubroutine(const char* subroutineFunctionName);

private:
	char* LoadTxtResource(const char* filename);
	void ShowShaderErrors(GLuint shaderID);
	void ShowLinkErrors(GLuint shaderID);

	GLuint shaderProgram;
	GLuint vertexShader;
	GLuint fragmentShader;
};
