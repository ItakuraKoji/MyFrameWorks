#pragma once

#include<unordered_map>
#include"ShaderClass.h"

//�V�F�[�_�[�N���X�����X�g�Ƃ��Ď����A����ӔC�����N���X
class ShaderList {
public:
	ShaderList();
	~ShaderList();
	bool Initialize();

	ShaderClass* GetShader(std::string shaderName);
	bool Add(std::string shaderName, std::string vertexShader, std::string pixelShader);
	bool Add(std::string shaderName, ShaderClass* shader);

private:
	std::unordered_map<std::string, ShaderClass*> shaderList;

};
