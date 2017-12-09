#pragma once
#include<GLEW\glew.h>
#include<fstream>
#include<string>


//�摜�ǂݍ��݃N���X�B3D���f���f�[�^�ƈقȂ�f�[�^�����Ȃ��̂Ń|�C���^�̏��n�Ƃ��͂��Ȃ�
class ImageLoader {
private:
	struct TGAHeader {
		char data1[12];
		short width;
		short height;
		char bpp;
		char data2;
	};
public:
	bool LoadTGAImage(const std::string& fileName, GLuint TextureID, unsigned int &returnWidth, unsigned int &returnHeight);
};