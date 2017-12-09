#include"FrameBuffer.h"

Framebuffer::Framebuffer(TextureList* list, const std::string& name, int width, int height) {
	Initialize(list, name, width, height);
}
Framebuffer::~Framebuffer() {
	Finalize();
}

void Framebuffer::Initialize(TextureList* list, const std::string& name, int width, int height) {
	//‚¢‚ë‚¢‚ë‚Æ‰¼‚ÌŽÀ‘•A×‚©‚­‚Íl‚¦‚È‚¢‚æ
	glGenFramebuffers(1, &this->frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);

	glGenRenderbuffers(1, &this->renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, this->renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->renderBuffer);

	list->AddEmptyTexture(name, width, height);

	Texture* texture = list->GetTexture(name);
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetTextureID(), 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->TextureName = name;
	this->textureWidth = width;
	this->textureHeight = height;
}

void Framebuffer::Finalize() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &this->frameBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glDeleteRenderbuffers(1, &this->renderBuffer);
}

void Framebuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
}
void Framebuffer::UnBind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int Framebuffer::GetWidth() {
	return this->textureWidth;
}
int Framebuffer::GetHeight() {
	return this->textureHeight;
}
