#include"ModelData.h"

////////
//public
////
MaterialData::MaterialData() {

}
MaterialData::~MaterialData() {
	for (auto i : this->materialArray) {
		for (int k = 0; k < i.size(); ++k) {
			if (!i[k].texture) {
				continue;
			}
			delete i[k].texture;
		}
	}
}

bool MaterialData::Initialize() {
	return true;
}

void MaterialData::Add(std::vector<Material> material) {
	this->materialArray.push_back(material);
}

float MaterialData::GetDiffuseColor() {
	return this->diffuseColor;
}
float MaterialData::GetAmbient() {
	return this->ambient;
}
float MaterialData::GetSpecularColor() {
	return this->specularColor;
}
float MaterialData::GetSpecularPower() {
	return this->specularPower;
}
Texture* MaterialData::GetTexture(int arrayIndex, int materialIndex) {
	return this->materialArray[arrayIndex][materialIndex].texture;
}

int MaterialData::GetNumMaterial(int arrayIndex) {
	return this->materialArray[arrayIndex].size();
}
int MaterialData::GetNumFace(int arrayIndex, int materialIndex) {
	return this->materialArray[arrayIndex][materialIndex].numFace;
}

