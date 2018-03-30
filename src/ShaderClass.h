#pragma once
#include<fstream>
#include<iostream>
#include<GLEW\glew.h>
#include<Eigen\Core>
#include"MyMathFanctions.h"

namespace K_Graphics {

	//�V�F�[�_�[�v���O�����̊Ǘ���uniform�ϐ��̎󂯓n����S������N���X
	//uniform�ϐ����͌Œ�I�ȋ@�\�������ƁA�֐����ɃL�����Ȃ��̂Ŕėp�I�Ȋ֐���p��
	class ShaderClass {
	public:
		ShaderClass(GLuint vertexShader, GLuint fragmentShader);
		~ShaderClass();

		bool Initialize(GLuint vertexShader, GLuint fragmentShader);
		void Finalize();

		void UseShader();
		void SetMatrix(const K_Math::Matrix4x4& mat);
		void SetWorldMatrix(const K_Math::Matrix4x4& world);
		void SetTexture(const std::string& uniformName, GLuint textureLayer, GLuint textureNumber);
		void SetDirectionalLight(float power, const K_Math::Vector4& color, const K_Math::Vector3& direction);
		void SetAmbientLight(float power, const K_Math::Vector4& color);

		//�ėp�I��uniform�ϐ��ւ̎󂯓n��
		void SetValue(const std::string& uniformName, int value);
		void SetValue(const std::string& uniformName, float value);
		void SetValue(const std::string& uniformName, const K_Math::Matrix4x4& value);
		void SetValue(const std::string& uniformName, const K_Math::Vector4& value);
		void SetValue(const std::string& uniformName, const K_Math::Vector3& value);
		void SetValue(const std::string& uniformName, const K_Math::Vector2& value);

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
}
