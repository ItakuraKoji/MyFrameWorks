#pragma once
#include<GLEW\glew.h>
#include"ImageLoader.h"

//外部から読み込まれた、あるいは作られたテクスチャを保持する
//このクラスは、保持しているテクスチャの解放責任を持つ
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