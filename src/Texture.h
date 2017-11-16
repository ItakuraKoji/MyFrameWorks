#pragma once
#include<GLEW\glew.h>
#include"ImageLoader.h"

//�O������ǂݍ��܂ꂽ�A���邢�͍��ꂽ�e�N�X�`����ێ�����
//���̃N���X�́A�ێ����Ă���e�N�X�`���̉���ӔC������
class Texture {
public:
	Texture();
	~Texture();
	bool Initialize();
	bool LoadImage(const char *filename);
	void SetImageData(void *data, int width, int height);
	GLuint GetTextureID();
	unsigned int GetWidth();
	unsigned int GetHeight();

private:
	GLuint textureID;
	unsigned int width;
	unsigned int height;
};