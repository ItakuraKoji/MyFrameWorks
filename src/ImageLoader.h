#pragma once
#include<GLEW\glew.h>
#include<fstream>
#include<string>


//画像読み込みクラス。3Dモデルデータと異なりデータが少ないのでポインタの譲渡とかはしない
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