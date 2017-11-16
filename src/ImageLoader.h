#pragma once
#include<GLEW\glew.h>
#include<fstream>

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
	bool LoadTGAImage(const char* filename, GLuint TextureID, unsigned int &returnWidth, unsigned int &returnHeight);
};