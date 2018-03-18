#include"ModelData.h"

////////
//public
////
BoneData::BoneData() {
	this->interporationCount = 0.0f;
	this->interporationMaxCount = 0.0f;
}
BoneData::~BoneData() {

}

bool BoneData::Initialize() {
	return true;
}

void BoneData::Add(std::vector<Bone> &boneData) {
	this->boneData.push_back(boneData);
	SetClurrentBoneData(this->boneData.size() - 1, 0);
}

void BoneData::SetMatrixTextureData(int arrayIndex, Texture *texture) {
	++this->interporationCount;
	if (this->interporationCount >= this->interporationMaxCount) {
		this->interporationMaxCount = 0.0f;
	}

	int numBone = (int)this->boneData[arrayIndex].size();
	float mat[16 * 120] = {};

	for (int i = 0; i < numBone; ++i) {
		int matIndex = i * 16;
		//行列補間
		if (this->interporationMaxCount) {
			BoneInterporation(arrayIndex, i, this->interporationCount / this->interporationMaxCount);
		}

		Matrix4f boneMat = CalculateBoneMatrix(arrayIndex, i);
		for (int k = 0; k < 16; ++k) {
			mat[matIndex + k] = boneMat(k);
		}
	}

	GLuint tex = texture->GetTextureID();
	texture->SetImageData(mat, numBone * 4, 1, TextureType::Float, TextureColorType::RGBA32F, TextureColorType::RGBA);
}

void BoneData::SetClurrentBoneData(int arrayIndex, int time) {
	FbxTime fbxTime;
	//60フレームのアニメーションを120として半分の速度での再生を可能にした
	fbxTime.SetTime(0, 0, 0, time, 0, FbxTime::eFrames120);


	int numBone = (int)this->boneData[arrayIndex].size();
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

void BoneData::StartInterporation(int frameCount) {
	for (int i = 0; i < this->boneData.size(); ++i) {
		for (int j = 0; j < this->boneData[i].size(); ++j) {
			this->boneData[i][j].interPolationMat = this->boneData[i][j].currentMat;
		}
	}
	this->interporationCount = 0.0f;
	this->interporationMaxCount = (float)frameCount;
}

int BoneData::GetNumBone(int arrayIndex) {
	return (int)this->boneData[arrayIndex].size();
}



////////
//private
////
void BoneData::BoneInterporation(int arrayIndex, int boneIndex, float ratio) {
	//平行移動取り出し
	Vector3f translationA = this->boneData[arrayIndex][boneIndex].interPolationMat.block(0, 3, 3, 1);
	Vector3f translationB = this->boneData[arrayIndex][boneIndex].currentMat.block(0, 3, 3, 1);

	//回転行列＆拡縮行列
	Matrix3f rotScaleA = this->boneData[arrayIndex][boneIndex].interPolationMat.block(0, 0, 3, 3);
	Matrix3f rotScaleB = this->boneData[arrayIndex][boneIndex].currentMat.block(0, 0, 3, 3);

	//拡縮はベクトルの長さで表される
	Vector3f scaleA;
	scaleA.x() = rotScaleA.block(0, 0, 1, 3).norm();
	scaleA.y() = rotScaleA.block(1, 0, 1, 3).norm();
	scaleA.z() = rotScaleA.block(2, 0, 1, 3).norm();
	Vector3f scaleB;
	scaleB.x() = rotScaleB.block(0, 0, 1, 3).norm();
	scaleB.y() = rotScaleB.block(1, 0, 1, 3).norm();
	scaleB.z() = rotScaleB.block(2, 0, 1, 3).norm();

	//スケールで回転行列を割ると回転行列のみになる
	rotScaleA.block(0, 0, 1, 3) = rotScaleA.block(0, 0, 1, 3) / scaleA.x();
	rotScaleA.block(1, 0, 1, 3) = rotScaleA.block(1, 0, 1, 3) / scaleA.y();
	rotScaleA.block(2, 0, 1, 3) = rotScaleA.block(2, 0, 1, 3) / scaleA.z();
	rotScaleB.block(0, 0, 1, 3) = rotScaleB.block(0, 0, 1, 3) / scaleB.x();
	rotScaleB.block(1, 0, 1, 3) = rotScaleB.block(1, 0, 1, 3) / scaleB.y();
	rotScaleB.block(2, 0, 1, 3) = rotScaleB.block(2, 0, 1, 3) / scaleB.z();

	//それをクォータニオンへ変換
	Quaternionf rotationA(rotScaleA);
	Quaternionf rotationB(rotScaleB);
	
	Vector3f resultTrans = translationA * (1.0f - ratio) + translationB * ratio;
	Vector3f resultScale = scaleA * (1.0f - ratio) + scaleB * ratio;
	Quaternionf resultRot = rotationA.slerp(ratio, rotationB);

	//SRT行列作成
	Matrix4f resultMat = Matrix4f::Identity();
	resultMat(0, 0) = resultScale.x();
	resultMat(1, 1) = resultScale.y();
	resultMat(2, 2) = resultScale.z();
	Matrix4f rotMat = Matrix4f::Identity();
	rotMat.block(0, 0, 3, 3) = resultRot.matrix();
	resultMat = resultMat * rotMat;
	resultMat.block(0, 3, 3, 1) = resultTrans;

	//格納
	this->boneData[arrayIndex][boneIndex].currentMat = resultMat;
}

Matrix4f BoneData::CalculateBoneMatrix(int arrayIndex, int boneIndex) {
	Matrix4f bind = this->boneData[arrayIndex][boneIndex].bindMat.inverse();
	Matrix4f current = this->boneData[arrayIndex][boneIndex].currentMat;
	//blenderの出力ではボーンは軸情報に従わないので補正(180度Y軸回転　→　90度X軸回転)
	Matrix3f rot = (AngleAxisf(M::DegToRad(90), Vector3f(1.0f, 0.0f, 0.0f)) * AngleAxisf(M::DegToRad(180), Vector3f(0.0f, 1.0f, 0.0f))).matrix();
	Matrix4f mat = Matrix4f::Identity();
	mat.block(0, 0, 3, 3) = rot;

	return current * bind * mat;
}