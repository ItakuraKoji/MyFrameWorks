#include"ModelData.h"

ModelDatas::ModelDatas() {
	this->fbxData = 0;
	this->vertexBuffer = 0;
	this->material = 0;
	this->bone = 0;
	this->animation = 0;
}
ModelDatas::~ModelDatas() {
	if (this->fbxData) {
		delete this->fbxData;
	}
	if (this->vertexBuffer) {
		delete this->vertexBuffer;
	}
	if (this->material) {
		delete this->material;
	}
	if (this->bone) {
		delete this->bone;
	}
	if (this->animation) {
		delete this->animation;
	}
}