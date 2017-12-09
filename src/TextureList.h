#pragma once

#include<unordered_map>
#include"Texture.h"

//テクスチャクラスをリストとして持ち、解放責任をもつクラス
class TextureList {
public:
	TextureList();
	~TextureList();

	void Initialize();
	void Finalize();
	Texture* GetTexture(const std::string& textureName);
	bool LoadTexture(const std::string& textureName, const std::string& fileName);
	bool AddEmptyTexture(const std::string& textureName, int textureWidth, int textureHeight);

private:
	void Add(const std::string& textureName, Texture* texture);

private:
	std::unordered_map<std::string, Texture*> textureList;

};
