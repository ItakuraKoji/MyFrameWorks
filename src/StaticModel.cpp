#include"StaticModel.h"

////////
//public
////
StaticModel::StaticModel(const char* fileName, TextureList* list) {
	this->vertexBuffers = 0;
	this->materialDatas = 0;
	if (!Initialize(fileName, list)) {
		Finalize();
		throw "“Ç‚Ýž‚ÝŽ¸”s";
	}
}
StaticModel::~StaticModel() {
	Finalize();
}

//‰Šú‰»
bool StaticModel::Initialize(const char* fileName, TextureList* list) {
	this->textureList = list;
	if (!LoadFbxModel(fileName)) {
		return false;
	}
	return true;
}

//ŠJ•ú
void StaticModel::Finalize() {
	if (this->vertexBuffers) {
		delete this->vertexBuffers;
	}
	if (this->materialDatas) {
		delete this->materialDatas;
	}
}

//•`‰æ
void StaticModel::Draw() {
	Matrix4f mat = this->projection * this->view * this->world;
	this->shader->SetMatrix(mat);
	this->shader->SetWorldMatrix(this->world);

	int numArray = this->vertexBuffers->GetNumBuffer();
	for (int i = 0; i < numArray; ++i) {
		DrawBuffers(i);
	}
}

//ƒCƒ“ƒXƒ^ƒ“ƒX•`‰æ
void StaticModel::InstanceDraw(int numInstance) {

}



////////
//private
////

bool StaticModel::LoadFbxModel(const char* fileName) {
	FbxModelLoader* loader = new FbxModelLoader;
	if (!loader->LoadFBX(fileName, this->textureList)) {
		delete loader;
		return false;
	}

	this->vertexBuffers = loader->PassVertexBuffer();
	this->materialDatas = loader->PassMaterialData();

	delete loader;
	return true;
}

void StaticModel::DrawBuffers(int arrayIndex) {
	GLuint VAO = this->vertexBuffers->GetVAO(arrayIndex);
	glBindVertexArray(VAO);

	int numMaterial = this->materialDatas->GetNumMaterial(arrayIndex);
	for (int k = 0; k < numMaterial; ++k) {
		Texture* texture = this->textureList->GetTexture(this->materialDatas->GetMaterial(arrayIndex, k).textureName);
		if (texture) {
			GLuint TextureID = texture->GetTextureID();
			this->shader->SetTexture("sampler", 0, TextureID);
		}
		GLuint IBO = this->vertexBuffers->GetIBO(arrayIndex, k);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, this->materialDatas->GetNumFace(arrayIndex, k) * 3, GL_UNSIGNED_INT, 0);

	}
}
