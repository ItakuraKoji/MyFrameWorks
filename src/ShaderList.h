#pragma once

#include<unordered_map>
#include"ShaderClass.h"

//�V�F�[�_�[�N���X�����X�g�Ƃ��Ď����A����ӔC�����N���X
class ShaderList {
public:
	ShaderList();
	~ShaderList();
	bool Initialize();
	void Finalize();

	ShaderClass* GetShader(const std::string& shaderName);
	ShaderClass* UseShader(const std::string& shaderName);
	bool Add(const std::string& shaderName, const std::string& vertexShader, const std::string& pixelShader);
	bool Add(const std::string& shaderName, ShaderClass* shader);

private:
	std::unordered_map<std::string, ShaderClass*> shaderList;

};
