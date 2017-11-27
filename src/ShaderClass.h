#pragma once
#include<fstream>
#include<iostream>
#include<GLEW\glew.h>
#include<Eigen\Core>

using namespace Eigen;

//�V�F�[�_�[�̊Ǘ���uniform�ϐ��̎󂯓n����S������N���X
//uniform�ϐ����͌Œ�I�ȋ@�\�������ƁA�֐����ɃL�����Ȃ��̂Ŕėp�I�Ȋ֐���p��
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

	//�ėp�I��uniform�ϐ��ւ̎󂯓n��
	void SetValue(const char* uniformName, int value);
	void SetValue(const char* uniformName, float value);
	void SetValue(const char* uniformName, Matrix4f& value);
	void SetValue(const char* uniformName, Vector4f& value);
	void SetValue(const char* uniformName, Vector3f& value);

	//subroutine�̐ݒ�(vertex shader)
	void SetVertexShaderSubroutine(const char* subroutineFunctionName);
	//subroutine�̐ݒ�(fragment shader)
	void SetFragmentShaderSubroutine(const char* subroutineFunctionName);

private:
	char* LoadTxtResource(const char* filename);
	void ShowShaderErrors(GLuint shaderID);
	void ShowLinkErrors(GLuint shaderID);

	GLuint shaderProgram;
	GLuint vertexShader;
	GLuint fragmentShader;
};
