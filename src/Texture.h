#pragma once
#include<GLEW\glew.h>
#include<string>
#include"ImageLoader.h"

namespace K_Graphics {
	//�e�N�X�`���������l�̌^
	enum TextureType {
		Unsigned_Byte = GL_UNSIGNED_BYTE,
		Float = GL_FLOAT,
	};
	//�e�N�X�`���ɓn�������̉���
	enum TextureColorType {
		RGB = GL_RGB,
		BGR = GL_BGR,
		RGBA32F = GL_RGBA32F,
		RGBA = GL_RGBA,
		BGRA = GL_BGRA,
		RED = GL_RED,
	};

	//�O������ǂݍ��܂ꂽ�A���邢�͍��ꂽ�e�N�X�`����ێ�����
	//���̃N���X�́A�ێ����Ă���e�N�X�`���̉���ӔC������
	class Texture {
	public:
		Texture();
		~Texture();
		bool Initialize();
		bool LoadImage(const std::string& fileName);
		void SetImageData(void *data, int width, int height, TextureType bType, TextureColorType textureColor, TextureColorType dataColor);
		void SetFilter(bool isFiltering);
		GLuint GetTextureID();
		unsigned int GetWidth();
		unsigned int GetHeight();

	private:
		GLuint textureID;
		unsigned int width;
		unsigned int height;
	};
}
