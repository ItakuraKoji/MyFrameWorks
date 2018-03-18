#pragma once
#include<fstream>
#include<iostream>
#include<GLEW\glew.h>
#include<Eigen\Core>

using namespace Eigen;

//シェーダープログラムの管理とuniform変数の受け渡しを担当するクラス
//uniform変数名は固定的な機能を除くと、関数化にキリがないので汎用的な関数を用意
class ShaderClass {
public:
	ShaderClass(GLuint vertexShader, GLuint fragmentShader);
	~ShaderClass();

	bool Initialize(GLuint vertexShader, GLuint fragmentShader);
	void Finalize();

	void UseShader();
	void SetMatrix          (const Matrix4f& mat);
	void SetWorldMatrix     (const Matrix4f& world);
	void SetTexture         (const std::string& uniformName, GLuint textureLayer, GLuint textureNumber);
	void SetDirectionalLight(float power, const Vector4f& color, const Vector3f& direction);
	void SetAmbientLight    (float power, const Vector4f& color);

	//汎用的なuniform変数への受け渡し
	void SetValue(const std::string& uniformName, int value);
	void SetValue(const std::string& uniformName, float value);
	void SetValue(const std::string& uniformName, const Matrix4f& value);
	void SetValue(const std::string& uniformName, const Vector4f& value);
	void SetValue(const std::string& uniformName, const Vector3f& value);
	void SetValue(const std::string& uniformName, const Vector2f& value);

	//subroutineの設定(vertex shader)
	void SetVertexShaderSubroutine(const std::string& subroutineFunctionName);
	//subroutineの設定(fragment shader)
	void SetFragmentShaderSubroutine(const std::string& subroutineFunctionName);

private:
	char* LoadTxtResource(const std::string& fileName);
	void ShowShaderErrors(GLuint shaderID);
	void ShowLinkErrors(GLuint shaderID);

	GLuint shaderProgram;
};
