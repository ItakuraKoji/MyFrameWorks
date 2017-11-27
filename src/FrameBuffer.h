#pragma once

#include<GLEW\glew.h>
#include"Texture.h"

//�e�X�g�p�B�I�t�X�N���[�������_�����O�̂��߂̃t���[���o�b�t�@�N���X
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