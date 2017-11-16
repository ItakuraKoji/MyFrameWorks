#include"ModelData.h"

////////
//public
////
FbxData::FbxData() {
	this->manager = NULL;
	this->importer = NULL;
	this->scene = NULL;
}
FbxData::~FbxData() {
	this->manager->Destroy();
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