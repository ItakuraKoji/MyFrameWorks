#pragma once

#include<GLEW\glew.h>
#include"TextureList.h"

//テスト用。オフスクリーンレンダリングのためのフレームバッファクラス(テクスチャ)
class Framebuffer {
public:
	Framebuffer(TextureList* list, const std::string& name, int width, int height);
	~Framebuffer();
	void Initialize(TextureList* list, const std::string& name, int width, int height);
	void Finalize();
	void Bind();
	void UnBind();
	int GetWidth();
	int GetHeight();

private:
	std::string TextureName;
	int textureWidth;
	int textureHeight;
	GLuint frameBuffer;
	GLuint renderBuffer;
};