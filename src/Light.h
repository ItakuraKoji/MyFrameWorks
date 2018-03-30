#pragma once

#include<Eigen\Core>
#include"ShaderClass.h"
#include"MyMathFanctions.h"

namespace K_Graphics {

	//���C�g�̐ݒ���L�^����N���X�Q�A���������N���X�ɂ܂Ƃ߂��ق����悳�����B�������Ȃ����C�g�Ƃ����肦�Ȃ����B

	class AmbientLight {
	public:
		AmbientLight();
		void SetLight(ShaderClass* shader);

		void SetPower(float power);
		void SetColor(float r, float g, float b, float a);

		float GetPower();
		const K_Math::Vector4& GetColor();

	protected:
		float    power;
		K_Math::Vector4 color;
	};

	class DirectionalLight {
	public:
		DirectionalLight();
		void SetLight(ShaderClass* shader);

		void SetPower(float power);
		void SetColor(float r, float g, float b, float a);

		float GetPower();
		const K_Math::Vector4& GetColor();

		void  SetDirection(float x, float y, float z);
		const K_Math::Vector3& GetDirection();

	protected:
		float    power;
		K_Math::Vector4 color;
		K_Math::Vector3 direction;
	};
}

