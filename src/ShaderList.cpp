#include"ShaderList.h"

ShaderList::ShaderList() {

}
ShaderList::~ShaderList() {
	for (auto it = this->shaderList.begin(); it != this->shaderList.end(); ++it) {
		delete (*it).second;
	}
}

bool ShaderList::Initialize() {
	return true;
}



//���X�g����V�F�[�_�[���擾�A������UseProgram()�A�L�[�����݂��Ȃ��Ƃ���NULL��Ԃ�
ShaderClass* ShaderList::UseShader(std::string name) {
	if (this->shaderList.find(name) == shaderList.end()) {
		return 0;
	}
	this->shaderList[name]->UseShader();
	return this->shaderList[name];
}

//���X�g�ւ̃V�F�[�_�[�̒ǉ�
bool ShaderList::Add(std::string name, ShaderClass* shader) {
	if (!shader) {
		return false;
	}
	this->shaderList[name] = shader;
	return true;
}
//�V�F�[�_�[�̓ǂݍ��݂�������Ƀ��X�g�֓o�^
bool ShaderList::Add(std::string name, std::string vertex, std::string pixel) {
	ShaderClass* newShader = new ShaderClass;
	if (!newShader) {
		return false;
	}
	if (!newShader->Initialize(vertex.data(), pixel.data())) {
		delete newShader;
		return false;
	}
	this->Add(name, newShader);

	return true;
}