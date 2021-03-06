#include"LightList.h"

namespace K_Graphics {

	LightList::LightList() {
		Initialize();
	}
	LightList::~LightList() {

	}

	void LightList::Initialize() {
		this->ambient.clear();
		this->directional.clear();
	}


	void LightList::SetAmbient(const std::string& lightName, ShaderClass* shader) {
		if (this->ambient.find(lightName) == this->ambient.end()) {
			return;
		}
		this->ambient[lightName].SetLight(shader);
	}
	void LightList::SetDirectional(const std::string& lightName, ShaderClass* shader) {
		if (this->directional.find(lightName) == this->directional.end()) {
			return;
		}
		this->directional[lightName].SetLight(shader);
	}

	void LightList::AddAmbient(const std::string& lightName, float power, K_Math::Vector4& color) {
		AmbientLight addLight;
		addLight.SetPower(power);
		addLight.SetColor(color.x(), color.y(), color.z(), color.w());
		this->ambient[lightName] = addLight;
	}
	void LightList::AddDirectional(const std::string& lightName, float power, K_Math::Vector4& color, K_Math::Vector3& direction) {
		DirectionalLight addLight;
		addLight.SetPower(power);
		addLight.SetColor(color.x(), color.y(), color.z(), color.w());
		addLight.SetDirection(direction.x(), direction.y(), direction.z());
		this->directional[lightName] = addLight;
	}
}
