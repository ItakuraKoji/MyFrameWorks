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

//‰Šú‰»
bool SkinModel::Initialize(const char* fileName) {
	if (!LoadFbxModel(fileName)) {
		return false;
	}
	this->boneTexture = new Texture;
	this->boneTexture->Initialize();

	this->animationDatas->SetAnimation("Idle", this->fbxData->GetScene(), false, true);

	return true;
}

//XV
void SkinModel::Run() {
	this->animationDatas->Run();

}

//•`‰æ
void SkinModel::Draw() {
	this->shader->SetShaderParameter(this->world.data(), this->view.data(), this->projection.data());

	int time = this->animationDatas->GetCurrentAnimTime();
	int numArray = this->vertexBuffers->GetNumBuffer();
	for (int i = 0; i < numArray; ++i) {
		this->boneDatas->SetClurrentBoneData(i, time);
		this->boneDatas->SetMatrixTextureData(i, this->boneTexture);
		this->shader->SetShaderTexture2(1, this->boneTexture->GetTextureID());
		this->shader->SetShaderNumBone(this->boneDatas->GetNumBone(i));

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
		this->shader->SetShaderTexture(0, TextureID);
		GLuint IBO = this->vertexBuffers->GetIBO(arrayIndex, k);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, this->materialDatas->GetNumFace(arrayIndex, k) * 3, GL_UNSIGNED_INT, 0);

	}
}
