#include"CameraList.h"

CameraList::CameraList() {
	Initialize();
}
CameraList::~CameraList() {
	Initialize();
}
void CameraList::Initialize() {
	for (auto i : this->cameraList) {
		delete i.second;
	}
	this->cameraList.clear();
}

//パースペクティブ投影
void CameraList::AddPerspectiveCamera(const std::string& name, int width, int height, float neer, float far, float fieldOfView) {
	if (this->cameraList.find(name) != this->cameraList.end()) {
		return;
	}
	this->cameraList[name] = new CameraClass(CameraType::Perspective, width, height, neer, far, fieldOfView);
}
//平行投影
void CameraList::AddOrthoCamera(const std::string& name, int width, int height, float neer, float far) {
	if (this->cameraList.find(name) != this->cameraList.end()) {
		return;
	}
	this->cameraList[name] = new CameraClass(CameraType::Ortho, width, height, neer, far, 0.0f);
}

CameraClass* CameraList::GetCamera(const std::string& name) {
	if (this->cameraList.find(name) == this->cameraList.end()) {
		return 0;
	}
	return this->cameraList[name];
}
