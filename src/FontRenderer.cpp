#include"FontRenderer.h"

FontRenderer::FontRenderer(const char* filePass) {
	try {
		setlocale(LC_ALL, "japanese");
		this->generator = new FontGenerator(filePass);
		this->sprite = new SpriteObject(nullptr);
	}
	catch(std::string& errorText){
		throw(errorText);
	}
}
FontRenderer::~FontRenderer() {
	if (this->generator != nullptr) {
		delete this->generator;
		this->generator = nullptr;
	}
	if (this->sprite != nullptr) {
		delete this->sprite;
		this->sprite = nullptr;
	}
}

void FontRenderer::DrawString(GameParameters* param, const std::string& text, int size, int screenX, int screenY) {
	wchar_t* buffer = new wchar_t[text.size() + 1];
	mbstowcs(buffer, text.data(), text.size() + 1);
	for (int i = 0; buffer[i] != L'\0'; ++i) {
		this->generator->SetFont(buffer[i], size);
	}
	Texture* texture = this->generator->GetFontAtlas(size);
	this->sprite->SetTexture(texture);

	int position = 0;
	for (int i = 0; buffer[i] != L'\0'; ++i) {
		FontGenerator::FontData data;
		this->generator->GetFontData(data, buffer[i], size);
		this->sprite->Draw(param, M::Box2D(screenX + position, screenY + data.fixPositionY, data.locationUV.w, data.locationUV.h), data.locationUV);
		position += data.advance;
	}
	delete buffer;
}

void FontRenderer::ClearFontTexture() {

}
