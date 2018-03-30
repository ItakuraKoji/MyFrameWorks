#pragma once

#include<unordered_map>
#include"Light.h"
#include"MyMathFanctions.h"

namespace K_Graphics {

	//���������܂Ƃ߂��N���X�A���������̓R�X�g�������̂Ő����i��
	//�A���r�G���g�F�P��
	//���̑��F�S�i�܂��\��j
	class LightList {
	public:
		LightList();
		~LightList();
		void Initialize();

		void SetAmbient(const std::string& lightName, ShaderClass* shader);
		void SetDirectional(const std::string& lightName, ShaderClass* shader);

		void AddAmbient(const std::string& lightName, float power, K_Math::Vector4& color);
		void AddDirectional(const std::string& lightName, float power, K_Math::Vector4& color, K_Math::Vector3& direction);

	private:
		std::unordered_map<std::string, AmbientLight> ambient;
		std::unordered_map<std::string, DirectionalLight> directional;
	};
}
