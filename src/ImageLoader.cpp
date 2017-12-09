#include"ImageLoader.h"

//TGAファイルを読み込み
bool ImageLoader::LoadTGAImage(const std::string& fileName, GLuint TextureID, unsigned int &returnWidth, unsigned int &returnHeight) {
	std::ifstream file;

	char* tgaImage;
	TGAHeader header;
	int width, height, bpp, imagesize;
	unsigned int numColor;
	GLenum glColorFormat, tgaColorFormat;


	file.open(fileName, std::ios::binary | std::ios::in);
	if (file.fail()) {
		return false;
	}
	file.read((char*)&header, sizeof(TGAHeader));


	width = (int)header.width;
	height = (int)header.height;
	bpp = (int)header.bpp;

	if (bpp == 32) {
		numColor = 4;
		glColorFormat = GL_RGBA;
		tgaColorFormat = GL_BGRA;
	}
	else if (bpp == 24) {
		numColor = 3;
		glColorFormat = GL_RGB;
		tgaColorFormat = GL_BGR;
	}

	imagesize = width * height * numColor;

	tgaImage = new char[imagesize];
	file.read((char*)tgaImage, imagesize);
	file.close();

	returnWidth = width;
	returnHeight = height;

	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, glColorFormat, width, height, 0, tgaColorFormat, GL_UNSIGNED_BYTE, tgaImage);

	//範囲外の表示
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//拡大縮小時の補完
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//ミップマップを作成
	glGenerateMipmap(GL_TEXTURE_2D);

	delete[] tgaImage;
	tgaImage = 0;

	return true;
}