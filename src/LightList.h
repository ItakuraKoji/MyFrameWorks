#pragma once

#include<unordered_map>
#include"Light.h"

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

	void AddAmbient(const std::string& lightName, float power, Vector4f& color);
	void AddDirectional(const std::string& lightName, float power, Vector4f& color, Vector3f& direction);

private:
	std::unordered_map<std::string, AmbientLight> ambient;
	std::unordered_map<std::string, DirectionalLight> directional;
};
