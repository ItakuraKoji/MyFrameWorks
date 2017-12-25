#pragma once

#include<unordered_map>
#include<string>
#include"CameraClass.h"

//ƒJƒƒ‰‚É–¼‘O‚ğ•t‚¯‚ÄŠÇ—
class CameraList {
public:
	CameraList();
	~CameraList();
	void Initialize();

	void AddPerspectiveCamera(const std::string& name, Vector3f& position, Vector3f& target, int width, int height, float neer, float far, float fieldOfView);
	void AddOrthoCamera      (const std::string& name, Vector3f& position, Vector3f& target, int width, int height, float neer, float far);
	CameraClass* GetCamera(const std::string& name);

private:
	std::unordered_map<std::string, CameraClass*> cameraList;
};
