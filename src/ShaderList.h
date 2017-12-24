#pragma once

#include<unordered_map>
#include"ShaderClass.h"

//シェーダークラスをリストとして持ち、解放責任をもつクラス
class ShaderList {
public:
	ShaderList();
	~ShaderList();
	bool Initialize();

	ShaderClass* GetShader(const std::string& shaderName);
	ShaderClass* UseShader(const std::string& shaderName);

	void AddVertexShader(const std::string& fileName);
	void AddPixelShader(const std::string& fileName);

	bool Add(const std::string& shaderName, const std::string& vertexShader, const std::string& pixelShader);

private:
	char* LoadTxtResource(const std::string& fileName);
	void ShowShaderErrors(GLuint shaderID);

private:
	std::unordered_map<std::string, ShaderClass*> shaderList;
	std::unordered_map<std::string, GLuint> vertexShaders;
	std::unordered_map<std::string, GLuint> pixelShaders;

};
