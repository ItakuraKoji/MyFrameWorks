#pragma once
#include<GLEW\glew.h>
#include<fstream>
#include<string>

namespace K_Loader {
	//�摜�ǂݍ��݃N���X�B3D���f���f�[�^�ƈقȂ�f�[�^�����Ȃ��̂Ń|�C���^�̏��n�Ƃ��͂��Ȃ�
	class ImageLoader {
	private:
		struct TGAHeader {
			char idSize;//�摜�T�C�Y�O��ID�̃T�C�Y
			char data1[1];//�p�f�B���O
			char type;//�摜�^�C�v
			char data2[9];//�p�f�B���O
			short width;//�摜��
			short height;//�摜����
			char bpp;//�摜�[�x
			char descriptor;//�摜�f�X�N���v�^
		};
	public:
		bool LoadTGAImage(const std::string& fileName, GLuint TextureID, unsigned int &returnWidth, unsigned int &returnHeight);

	private:
		void SetTgaData(char* data, char* src, int width, int height, int pixelDepth, bool xReverse, bool yReverse);
		void DecodeRLEImage(char* data, char* src, int width, int height, int pixelDepth);
	};
}
