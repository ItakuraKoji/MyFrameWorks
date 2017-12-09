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



//リストからシェーダーを取得、そしてUseProgram()、キーが存在しないときはNULLを返す
ShaderClass* ShaderList::UseShader(const std::string& name) {
	if (this->shaderList.find(name) == shaderList.end()) {
		return 0;
	}
	this->shaderList[name]->UseShader();
	return this->shaderList[name];
}
//リストからシェーダーを取得、キーが存在しないときはNULLを返す
ShaderClass* ShaderList::GetShader(const std::string& name) {
	if (this->shaderList.find(name) == shaderList.end()) {
		return 0;
	}
	return this->shaderList[name];
}

//リストへのシェーダーの追加
bool ShaderList::Add(const std::string& name, ShaderClass* shader) {
	if (!shader) {
		return false;
	}
	this->shaderList[name] = shader;
	return true;
}
//シェーダーの読み込みをした後にリストへ登録
bool ShaderList::Add(const std::string& name, const std::string& vertex, const std::string& pixel) {
	ShaderClass* newShader = new ShaderClass(vertex.data(), pixel.data());
	if (!newShader) {
		return false;
	}
	this->Add(name, newShader);

	return true;
}