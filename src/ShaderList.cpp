#include"ShaderList.h"

ShaderList::ShaderList() {
	Initialize();
}
ShaderList::~ShaderList() {
	Finalize();
}

bool ShaderList::Initialize() {
	Finalize();
	return true;
}
void ShaderList::Finalize() {
	for (auto it = this->shaderList.begin(); it != this->shaderList.end(); ++it) {
		delete (*it).second;
	}
	this->shaderList.clear();
}



//���X�g����V�F�[�_�[���擾�A������UseProgram()�A�L�[�����݂��Ȃ��Ƃ���NULL��Ԃ�
ShaderClass* ShaderList::UseShader(const std::string& name) {
	if (this->shaderList.find(name) == shaderList.end()) {
		return 0;
	}
	this->shaderList[name]->UseShader();
	return this->shaderList[name];
}
//���X�g����V�F�[�_�[���擾�A�L�[�����݂��Ȃ��Ƃ���NULL��Ԃ�
ShaderClass* ShaderList::GetShader(const std::string& name) {
	if (this->shaderList.find(name) == shaderList.end()) {
		return 0;
	}
	return this->shaderList[name];
}

//���X�g�ւ̃V�F�[�_�[�̒ǉ�
bool ShaderList::Add(const std::string& name, ShaderClass* shader) {
	if (!shader) {
		return false;
	}
	this->shaderList[name] = shader;
	return true;
}
//�V�F�[�_�[�̓ǂݍ��݂�������Ƀ��X�g�֓o�^
bool ShaderList::Add(const std::string& name, const std::string& vertex, const std::string& pixel) {
	ShaderClass* newShader = new ShaderClass(vertex.data(), pixel.data());
	if (!newShader) {
		return false;
	}
	this->Add(name, newShader);

	return true;
}