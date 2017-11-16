#include"ModelData.h"

////////
//public
////
BoneData::BoneData() {

}
BoneData::~BoneData() {

}

bool BoneData::Initialize() {
	return true;
}

void BoneData::Add(std::vector<Bone> &boneData) {
	this->boneData.push_back(boneData);
}

void BoneData::SetMatrixTextureData(int arrayIndex, Texture *texture) {
	int numBone = this->boneData[arrayIndex].size();
	float mat[16 * 70] = {};

	for (int i = 0; i < numBone; ++i) {
		int matIndex = i * 16;
		Matrix4f boneMat = CalculateBoneMatrix(arrayIndex, i);
		for (int k = 0; k < 16; ++k) {
			mat[matIndex + k] = boneMat(k);
		}
	}

	GLuint tex = texture->GetTextureID();
	texture->SetImageData(mat, numBone * 4, 1);
}

void BoneData::SetClurrentBoneData(int arrayIndex, int time) {
	FbxTime fbxTime;
	//60フレームのアニメーションを120として半分の速度での再生を可能にした
	fbxTime.SetTime(0, 0, 0, time, 0, FbxTime::eFrames120);


	int numBone = this->boneData[arrayIndex].size();
	for (int k = 0; k < numBone; ++k) {
		FbxAMatrix mat;
		Matrix4f currentBone;
		mat = this->boneData[arrayIndex][k].cluster->GetLink()->EvaluateGlobalTransform(fbxTime);
		for (int x = 0; x < 4; ++x) {
			for (int y = 0; y < 4; ++y) {
				currentBone(x, y) = (float)mat.Get(y, x);
			}
		}
		this->boneData[arrayIndex][k].currentMat = currentBone;
	}
}

int BoneData::GetNumBone(int arrayIndex) {
	return (int)this->boneData[arrayIndex].size();
}



////////
//private
////
void BoneData::BoneInterporation(int arrayIndex, float ratio) {

}
Matrix4f BoneData::CalculateBoneMatrix(int arrayIndex, int boneIndex) {
	Matrix4f bind = this->boneData[arrayIndex][boneIndex].bindMat.inverse();
	Matrix4f current = this->boneData[arrayIndex][boneIndex].currentMat;
	return current * bind;
}