#include"StaticModel.h"

////////
//public
////
StaticModel::StaticModel() {

}
StaticModel::~StaticModel() {
	if (this->vertexBuffers) {
		delete this->vertexBuffers;
	}
	if (this->materialDatas) {
		delete this->materialDatas;
	}
	if (this->fbxData) {
		delete this->fbxData;
	}
}

//‰Šú‰»
bool StaticModel::Initialize(const char* fileName) {
	if (!LoadFbxModel(fileName)) {
		return false;
	}
	return true;
}

//XV
void StaticModel::Run() {

}

//•`‰æ
void StaticModel::Draw() {
	this->shader->SetShaderParameter(this->world.data(), this->view.data(), this->projection.data());

	int numArray = this->vertexBuffers->GetNumBuffer();
	for (int i = 0; i < numArray; ++i) {
		DrawBuffers(i);
	}
}



////////
//private
////

bool StaticModel::LoadFbxModel(const char* fileName) {
	FbxModelLoader *loader;
	loader = new FbxModelLoader;
	if (!loader->Initialize(fileName)) {
		return false;
	}

	this->fbxData       = loader->PassFbxData();
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
		if (this->materialDatas->GetTexture(arrayIndex, k)) {
			GLuint TextureID = this->materialDatas->GetTexture(arrayIndex, k)->GetTextureID();
			this->shader->SetShaderTexture(0, TextureID);
		}
		GLuint IBO = this->vertexBuffers->GetIBO(arrayIndex, k);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, this->materialDatas->GetNumFace(arrayIndex, k) * 3, GL_UNSIGNED_INT, 0);

	}
}
