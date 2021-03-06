#pragma once

#include<unordered_map>
#include<string>
#include"CameraClass.h"

namespace K_Graphics {

	//カメラに名前を付けて管理
	class CameraList {
	public:
		CameraList();
		~CameraList();
		void Initialize();

		void AddPerspectiveCamera(const std::string& name, K_Math::Vector3& position, K_Math::Vector3& target, int width, int height, float neer, float far, float fieldOfView);
		void AddOrthoCamera(const std::string& name, K_Math::Vector3& position, K_Math::Vector3& target, int width, int height, float neer, float far);
		CameraClass* GetCamera(const std::string& name);

	private:
		std::unordered_map<std::string, CameraClass*> cameraList;
	};
}
