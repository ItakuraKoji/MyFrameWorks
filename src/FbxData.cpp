#include"ModelData.h"

////////
//public
////
FbxData::FbxData() {
	this->manager = 0;
	this->importer = 0;
	this->scene = 0;
}
FbxData::~FbxData() {
	if (this->manager != nullptr) {
		this->manager->Destroy();
	}
}

bool FbxData::Initialize() {
	return true;
}

void FbxData::Add(FbxManager* manager, FbxImporter *importer, FbxScene *scene) {
	this->manager = manager;
	this->importer = importer;
	this->scene = scene;
}

FbxScene* FbxData::GetScene() {
	return this->scene;
}