#include"FrameBuffer.h"

Framebuffer::Framebuffer() {
	//‚¢‚ë‚¢‚ë‚Æ‰¼‚ÌŽÀ‘•A×‚©‚­‚Íl‚¦‚È‚¢‚æ
	glGenFramebuffers(1, &this->frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);

	glGenRenderbuffers(1, &this->renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, this->renderBuffer);

	this->renderTexture = new Texture;
	this->renderTexture->Initialize();
	this->renderTexture->SetImageData(0, 800, 800);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_BUFFER_BIT, GL_COLOR_ATTACHMENT0, this->renderTexture->GetTextureID(), 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
Framebuffer::~Framebuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &this->frameBuffer);
	if (this->renderTexture) {
		delete this->renderTexture;
	}
}

GLuint Framebuffer::GetTextureID() {
	return this->renderTexture->GetTextureID();
}

void Framebuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
}
void Framebuffer::UnBind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

