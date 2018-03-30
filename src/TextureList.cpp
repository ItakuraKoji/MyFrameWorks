#include"TextureList.h"

namespace K_Graphics {

	////////
	//public
	////
	TextureList::TextureList() {
		Initialize();
	}
	TextureList::~TextureList() {
		Finalize();
	}

	void TextureList::Initialize() {
		this->textureList.clear();
	}
	void TextureList::Finalize() {
		for (auto it = this->textureList.begin(); it != this->textureList.end(); ++it) {
			delete (*it).second;
		}
		this->textureList.clear();
	}

	Texture* TextureList::GetTexture(const std::string& textureName) {
		if (this->textureList.find(textureName) == this->textureList.end()) {
			return nullptr;
		}
		return this->textureList[textureName];
	}
	bool TextureList::LoadTexture(const std::string& textureName, const std::string& fileName) {
		//�������O�̍ēo�^�͔F�߂Ȃ��i�����e�N�X�`���͈�ɂ܂Ƃ܂�j
		if (this->textureList.find(textureName) != this->textureList.end()) {
			return true;
		}

		Texture* texture = new Texture;
		texture->Initialize();
		if (!texture->LoadImage(fileName.data())) {
			delete texture;
			return false;
		}

		this->textureList[textureName] = texture;
		return true;
	}
	bool TextureList::AddEmptyTexture(const std::string& textureName, int textureWidth, int textureHeight) {
		//�������O�̍ēo�^�͔F�߂Ȃ��i�����e�N�X�`���͈�ɂ܂Ƃ܂�j
		if (this->textureList.find(textureName) != this->textureList.end()) {
			return false;
		}

		Texture* texture = new Texture;
		texture->Initialize();
		texture->SetImageData(NULL, textureWidth, textureHeight, TextureType::Unsigned_Byte, TextureColorType::RGBA32F, TextureColorType::RGBA);

		this->textureList[textureName] = texture;
		return true;
	}

	void TextureList::DeleteTexture(const std::string& textureName) {
		//�������͍폜�ł��Ȃ�
		if (this->textureList.find(textureName) == this->textureList.end()) {
			return;
		}

		delete this->textureList[textureName];
		this->textureList.erase(textureName);
	}

}