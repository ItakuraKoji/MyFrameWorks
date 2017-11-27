#include"SkinModel.h"
////////
//public
////
SkinModel::SkinModel() {

}
SkinModel::~SkinModel() {
	if (this->vertexBuffers) {
		delete this->vertexBuffers;
	}
	if (this->materialDatas) {
		delete this->materialDatas;
	}
	if (this->fbxData) {
		delete this->fbxData;
	}
	if (this->animationDatas) {
		delete this->animationDatas;
	}
	if (this->boneDatas) {
		delete this->boneDatas;
	}
	if (this->boneTexture) {
		delete this->boneTexture;
	}
}

//初期化
bool SkinModel::Initialize(const char* fileName) {
	if (!LoadFbxModel(fileName)) {
		return false;
	}
	this->boneTexture = new Texture;
	this->boneTexture->Initialize();

	this->animationDatas->SetAnimation("Idle", this->fbxData->GetScene(), false, true);

	return true;
}

//更新
void SkinModel::Run() {
	this->animationDatas->Run();

}

//描画
void SkinModel::Draw() {
	Matrix4f mat = this->projection * this->view * this->world;
	this->shader->SetMatrix(mat);
	this->shader->SetWorldMatrix(this->world);

	int time = this->animationDatas->GetCurrentAnimTime();
	int numArray = this->vertexBuffers->GetNumBuffer();
	for (int i = 0; i < numArray; ++i) {
		this->boneDatas->SetClurrentBoneData(i, time);
		this->boneDatas->SetMatrixTextureData(i, this->boneTexture);
		this->shader->SetTexture("boneTex", 1, this->boneTexture->GetTextureID());
		this->shader->SetValue("numBone", this->boneDatas->GetNumBone(i));

		DrawBuffers(i);
	}
}



////////
//private
////

bool SkinModel::LoadFbxModel(const char* fileName) {
	FbxModelLoader *loader;
	loader = new FbxModelLoader;
	if (!loader->Initialize(fileName)) {
		return false;
	}

	this->fbxData        = loader->PassFbxData();
	this->vertexBuffers  = loader->PassVertexBuffer();
	this->materialDatas  = loader->PassMaterialData();
	this->boneDatas      = loader->PassBoneData();
	this->animationDatas = loader->PassAnimationData();

	delete loader;
	return true;
}

void SkinModel::DrawBuffers(int arrayIndex) {
	GLuint VAO = this->vertexBuffers->GetVAO(arrayIndex);
	glBindVertexArray(VAO);

	int numMaterial = this->materialDatas->GetNumMaterial(arrayIndex);
	for (int k = 0; k < numMaterial; ++k) {

		GLuint TextureID = this->materialDatas->GetTexture(arrayIndex, k)->GetTextureID();
		this->shader->SetTexture("sampler", 0, TextureID);
		GLuint IBO = this->vertexBuffers->GetIBO(arrayIndex, k);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, this->materialDatas->GetNumFace(arrayIndex, k) * 3, GL_UNSIGNED_INT, 0);

	}
}
