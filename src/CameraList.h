#pragma once

#include<unordered_map>
#include<string>
#include"CameraClass.h"

//�J�����ɖ��O��t���ĊǗ�
class CameraList {
public:
	CameraList();
	~CameraList();
	void Initialize();

	void AddPerspectiveCamera(const std::string& name, int width, int height, float neer, float far, float fieldOfView);
	void AddOrthoCamera      (const std::string& name, int width, int height, float neer, float far);
	CameraClass* GetCamera(const std::string& name);

private:
	std::unordered_map<std::string, CameraClass*> cameraList;
};
