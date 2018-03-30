#include"Texture.h"

namespace K_Graphics {
	////////
	//public
	////
	Texture::Texture() {
		Initialize();
	}
	Texture::~Texture() {
		glDeleteTextures(1, &this->textureID);
	}

	bool Texture::Initialize() {
		glGenTextures(1, &this->textureID);
		glBindTexture(GL_TEXTURE_2D, this->textureID);
		//�摜���A�^�b�`(��)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		//�摜�̊O���̕`��ɂ��Đݒ�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		//�g��k�����̕⊮�ɂ��Đݒ�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		return true;
	}

	bool Texture::LoadImage(const std::string& fileName) {
		K_Loader::ImageLoader loader;
		if (!loader.LoadTGAImage(fileName, this->textureID, this->width, this->height)) {
			return false;
		}
		return true;
	}

	//�摜�f�[�^��n���i��f�[�^���\�j
	void Texture::SetImageData(void *data, int width, int height, TextureType bType, TextureColorType textureColor, TextureColorType dataColor) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, textureColor, width, height, 0, dataColor, bType, data);
		//�摜�̊O���̕`��ɂ��Đݒ�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		//�g��k�����̕⊮�ɂ��Đݒ�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		this->width = width;
		this->height = height;
	}

	void Texture::SetFilter(bool isFiltering) {
		if (isFiltering) {
			//�g��k�����̕⊮
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			//�~�b�v�}�b�v���쐬
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			//�g��k�����̕⊮�ɂ��Đݒ�
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
	}


	GLuint Texture::GetTextureID() {
		return this->textureID;
	}
	unsigned int Texture::GetWidth() {
		return this->width;
	}
	unsigned int Texture::GetHeight() {
		return this->height;
	}

}
