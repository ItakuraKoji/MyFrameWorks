#include"SkinModel.h"
////////
//public
////
SkinModel::SkinModel(const char* fileName, TextureList* list) {
	this->fbxData = 0;
	this->vertexBuffers = 0;
	this->materialDatas = 0;
	this->animationDatas = 0;
	this->boneDatas = 0;
	this->boneTexture = 0;
	if (!Initialize(fileName, list)) {
		Finalize();
		throw "“Ç‚Ýž‚ÝŽ¸”s";
	}
}
SkinModel::~SkinModel() {
	Finalize();
}

//‰Šú‰»
bool SkinModel::Initialize(const char* fileName, TextureList* list) {
	this->textureList = list;
	if (!LoadFbxModel(fileName)) {
		return false;
	}
	this->boneTexture = new Texture;
	this->boneTexture->Initialize();

	//this->animationDatas->SetAnimation("Idle", this->fbxData->GetScene(), false, true);

	return true;
}

//ŠJ•ú
void SkinModel::Finalize() {
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

void SkinModel::BindVAO() {
	glBindVertexArray(this->vertexBuffers->GetVAO(0));
}

void SkinModel::SetAnimation(std::string animationName, bool isLoop, bool isInterporation, bool playOnce) {
	this->animationDatas->SetAnimation(animationName, this->fbxData->GetScene(), false, true);
}
void SkinModel::SetSpeed(int speed) {
	this->animationDatas->SetSpeed(speed);
}

//•`‰æ
void SkinModel::Draw() {
	if (this->boneDatas) {
		this->animationDatas->UpdateAnimation();
	}

	Matrix4f mat = this->projection * this->view * this->world;
	this->shader->SetMatrix(mat);
	this->shader->SetWorldMatrix(this->world);

	int numArray = this->vertexBuffers->GetNumBuffer();
	for (int i = 0; i < numArray; ++i) {
		if (this->boneDatas) {
			SetBone(i);
		}
		DrawBuffers(i);
	}
}

//ƒCƒ“ƒXƒ^ƒ“ƒX•`‰æ(ƒƒbƒVƒ…ŠK‘w‚Ìˆê”Ôã‚Ìˆê‚Â–Ú‚Ìƒ}ƒeƒŠƒAƒ‹‚Ì‚Ý)
void SkinModel::InstanceDraw(int numInstance) {
	Matrix4f mat = this->projection * this->view * this->world;
	this->shader->SetMatrix(mat);
	this->shader->SetWorldMatrix(this->world);

	int numArray = this->vertexBuffers->GetNumBuffer();
	GLuint VAO = this->vertexBuffers->GetVAO(0);
	glBindVertexArray(VAO);

	int numMaterial = this->materialDatas->GetNumMaterial(0);
	Texture* texture = this->textureList->GetTexture(this->materialDatas->GetMaterial(0, 0).textureName);
	GLuint TextureID = texture->GetTextureID();
	this->shader->SetTexture("sampler", 0, TextureID);
	GLuint IBO = this->vertexBuffers->GetIBO(0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElementsInstanced(GL_TRIANGLES, this->materialDatas->GetNumFace(0, 0) * 3, GL_UNSIGNED_INT, 0, numInstance);
}



////////
//private
////

bool SkinModel::LoadFbxModel(const char* fileName) {
	FbxModelLoader* loader = new FbxModelLoader;
	if (!loader->LoadFBX(fileName, this->textureList)) {
		delete loader;
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

void SkinModel::SetBone(int arrayIndex) {
	int time = this->animationDatas->GetCurrentAnimTime();
	this->boneDatas->SetClurrentBoneData(arrayIndex, time);
	this->boneDatas->SetMatrixTextureData(arrayIndex, this->boneTexture);
	this->shader->SetTexture("boneTex", 1, this->boneTexture->GetTextureID());
	this->shader->SetValue("numBone", this->boneDatas->GetNumBone(arrayIndex));
}

void SkinModel::DrawBuffers(int arrayIndex) {
	GLuint VAO = this->vertexBuffers->GetVAO(arrayIndex);
	glBindVertexArray(VAO);

	int numMaterial = this->materialDatas->GetNumMaterial(arrayIndex);
	for (int k = 0; k < numMaterial; ++k) {
		Texture* texture = this->textureList->GetTexture(this->materialDatas->GetMaterial(arrayIndex, k).textureName);
		GLuint TextureID = texture->GetTextureID();
		this->shader->SetTexture("sampler", 0, TextureID);
		GLuint IBO = this->vertexBuffers->GetIBO(arrayIndex, k);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, this->materialDatas->GetNumFace(arrayIndex, k) * 3, GL_UNSIGNED_INT, 0);

	}
}
