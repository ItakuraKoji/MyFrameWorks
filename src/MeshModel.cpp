#include"MeshModel.h"
////////
//public
////
MeshModel::MeshModel(ModelDatas* data) {
	this->data = 0;
	this->boneTexture = 0;
	if (!Initialize(data)) {
		Finalize();
		throw "���f���f�[�^���s�\���ł��A�ǂݍ��݂��ł��Ă��邩�m�F���Ă�������";
	}
}
MeshModel::~MeshModel() {
	Finalize();
}

//������
bool MeshModel::Initialize(ModelDatas* data) {

	//���_�ƃ}�e���A���͕K�{
	if (!data->vertexBuffer && !data->material) {
		return false;
	}
	this->data = data;
	this->boneTexture = new Texture;
	this->boneTexture->Initialize();
	return true;
}

//�J��
void MeshModel::Finalize() {
	if (this->data) {
		delete data;
	}
	if (this->boneTexture) {
		delete this->boneTexture;
	}
}

void MeshModel::BindVAO() {
	glBindVertexArray(this->data->vertexBuffer->GetVAO(0));
}

void MeshModel::SetAnimation(const std::string& animationName, bool isLoop, bool isInterporation, bool playOnce) {
	this->data->animation->SetAnimation(animationName, this->data->fbxData->GetScene(), false, true);
}
void MeshModel::SetSpeed(int speed) {
	this->data->animation->SetSpeed(speed);
}

//�`��
void MeshModel::Draw(DrawParameters& param, const std::string& shaderName) {
	if (this->data->bone) {
		this->data->animation->UpdateAnimation();
	}
	int numArray = this->data->vertexBuffer->GetNumBuffer();
	for (int i = 0; i < numArray; ++i) {
		if (this->data->bone) {
			SetBone(i, param, shaderName);
		}
		DrawBuffers(i, param, shaderName);
	}
}

//�C���X�^���X�`��(���b�V���K�w�̈�ԏ�̈�ڂ̃}�e���A���̂�)
void MeshModel::InstanceDraw(int numInstance, DrawParameters& param, const std::string& shaderName) {

	int numArray = this->data->vertexBuffer->GetNumBuffer();
	GLuint VAO = this->data->vertexBuffer->GetVAO(0);
	glBindVertexArray(VAO);

	ShaderClass* shader = param.shaderList->UseShader(shaderName);
	int numMaterial = this->data->material->GetNumMaterial(0);
	Texture* texture = param.textureList->GetTexture(this->data->material->GetMaterial(0, 0).textureName);
	GLuint TextureID = texture->GetTextureID();
	shader->SetTexture("sampler", 0, TextureID);
	GLuint IBO = this->data->vertexBuffer->GetIBO(0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElementsInstanced(GL_TRIANGLES, this->data->material->GetNumFace(0, 0) * 3, GL_UNSIGNED_INT, 0, numInstance);
}



////////
//private
////

void MeshModel::SetBone(int arrayIndex, DrawParameters& param, const std::string& shaderName) {
	int time = this->data->animation->GetCurrentAnimTime();
	this->data->bone->SetClurrentBoneData(arrayIndex, time);
	this->data->bone->SetMatrixTextureData(arrayIndex, this->boneTexture);

	ShaderClass* shader = param.shaderList->GetShader(shaderName);
	shader->SetTexture("boneTex", 1, this->boneTexture->GetTextureID());
	shader->SetValue("numBone", this->data->bone->GetNumBone(arrayIndex));
}

void MeshModel::DrawBuffers(int arrayIndex, DrawParameters& param, const std::string& shaderName) {
	GLuint VAO = this->data->vertexBuffer->GetVAO(arrayIndex);
	glBindVertexArray(VAO);

	ShaderClass* shader = param.shaderList->GetShader(shaderName);
	int numMaterial = this->data->material->GetNumMaterial(arrayIndex);
	for (int k = 0; k < numMaterial; ++k) {
		Texture* texture = param.textureList->GetTexture(this->data->material->GetMaterial(arrayIndex, k).textureName);
		GLuint TextureID = texture->GetTextureID();
		shader->SetTexture("sampler", 0, TextureID);
		GLuint IBO = this->data->vertexBuffer->GetIBO(arrayIndex, k);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, this->data->material->GetNumFace(arrayIndex, k) * 3, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}