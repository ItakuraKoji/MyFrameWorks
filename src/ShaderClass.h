#pragma once
#include<fstream>
#include<iostream>
#include<GLEW\glew.h>
#include<Eigen\Core>

using namespace Eigen;

//�V�F�[�_�[�v���O�����̊Ǘ���uniform�ϐ��̎󂯓n����S������N���X
//uniform�ϐ����͌Œ�I�ȋ@�\�������ƁA�֐����ɃL�����Ȃ��̂Ŕėp�I�Ȋ֐���p��
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

	//�ėp�I��uniform�ϐ��ւ̎󂯓n��
	void SetValue(const std::string& uniformName, int value);
	void SetValue(const std::string& uniformName, float value);
	void SetValue(const std::string& uniformName, const Matrix4f& value);
	void SetValue(const std::string& uniformName, const Vector4f& value);
	void SetValue(const std::string& uniformName, const Vector3f& value);
	void SetValue(const std::string& uniformName, const Vector2f& value);

	//subroutine�̐ݒ�(vertex shader)
	void SetVertexShaderSubroutine(const std::string& subroutineFunctionName);
	//subroutine�̐ݒ�(fragment shader)
	void SetFragmentShaderSubroutine(const std::string& subroutineFunctionName);

private:
	char* LoadTxtResource(const std::string& fileName);
	void ShowShaderErrors(GLuint shaderID);
	void ShowLinkErrors(GLuint shaderID);

	GLuint shaderProgram;
};
