#pragma once

#include<GLEW\glew.h>
#include"Texture.h"

//テスト用。オフスクリーンレンダリングのためのフレームバッファクラス
class Framebuffer {
public:
	Framebuffer();
	~Framebuffer();
	void Bind();
	void UnBind();
	GLuint GetTextureID();

private:
	GLuint frameBuffer;
	GLuint renderBuffer;
	Texture* renderTexture;

};