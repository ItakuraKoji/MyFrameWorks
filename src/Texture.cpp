#include"Texture.h"

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
	ImageLoader loader;
	if (!loader.LoadTGAImage(fileName, this->textureID, this->width, this->height)) {
		return false;
	}
	return true;
}

//�摜�f�[�^��n���i��f�[�^���\�j
void Texture::SetImageData(void *data, int width, int height) {
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, data);
	//�摜�̊O���̕`��ɂ��Đݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//�g��k�����̕⊮�ɂ��Đݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	this->width = width;
	this->height = height;
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
