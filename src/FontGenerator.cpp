#include"FontGenerator.h"

namespace K_Graphics {

	////////
	//public
	////

	FontGenerator::FontGenerator(const char* fontFilePass) {
		FT_Init_FreeType(&this->ftLib);
		if (this->ftLib == nullptr) {
			throw("FreeType Initialize is Failed");
		}
		FT_New_Face(this->ftLib, fontFilePass, 0, &this->ftFace);
		if (this->ftFace == nullptr) {
			throw("FontFile Load is Failed : " + std::string(fontFilePass));
		}
	}
	FontGenerator::~FontGenerator() {
		ClearAllFontAtlas();
		FT_Done_Face(this->ftFace);
		FT_Done_FreeType(this->ftLib);
	}

	void FontGenerator::SetFont(wchar_t character, int fontSize) {
		//�T�C�Y���͂��߂Ďw�肳�ꂽ�Ƃ��̓e�N�X�`���V�K�쐬
		if (this->fontAtlas.find(fontSize) == this->fontAtlas.end()) {
			CreateNewTexture(fontSize);
		}

		//���߂đł��ꂽ�����i�T�C�Y�Ⴂ���قȂ���̂Ƃ���j�̓r�b�g�}�b�v��V�K�쐬��Ƀe�N�X�`���ɑg�ݍ���
		if (ExistFont(character, fontSize)) {
			return;
		}

		//�����e�N�X�`�����쐬
		CreateNewFontAtlas(character, fontSize);
	}

	Texture* FontGenerator::GetFontAtlas(int fontSize) {
		if (this->fontAtlas.find(fontSize) == this->fontAtlas.end()) {
			return nullptr;
		}
		return this->fontAtlas[fontSize].texture;
	}


	void FontGenerator::ClearFontAtlas(int fontSize) {
		if (this->fontAtlas.find(fontSize) != this->fontAtlas.end()) {
			return;
		}
		FontAtlas *atlas = &this->fontAtlas[fontSize];
		if (atlas->buffer != nullptr) {
			delete[] atlas->buffer;
		}
		if (atlas->texture != nullptr) {
			delete atlas->texture;
		}
		this->fontAtlas.erase(fontSize);
		for (auto i : this->fontData) {
			i.second.fontData.erase(fontSize);
		}

	}

	void FontGenerator::ClearAllFontAtlas() {
		for (auto i : this->fontAtlas) {
			if (i.second.buffer) {
				delete[] i.second.buffer;
			}
			if (i.second.texture) {
				delete i.second.texture;
			}
		}
		for (auto i : this->fontData) {
			i.second.fontData.clear();
		}
		this->fontAtlas.clear();
		this->fontData.clear();
	}

	bool FontGenerator::GetFontData(FontData& result, wchar_t font, int fontSize) {
		if (this->fontData.find(font) == this->fontData.end()) {
			return false;
		}
		if (this->fontData[font].fontData.find(fontSize) == this->fontData[font].fontData.end()) {
			return false;
		}
		result = this->fontData[font].fontData[fontSize];
		return true;
	}


	////////
	//private
	////

	void FontGenerator::ShowPixelType(FT_Bitmap bitmap) {
		switch (bitmap.pixel_mode) {
		case FT_PIXEL_MODE_NONE:
			printf("FT_PIXEL_MODE_NONE\n");
			break;
		case FT_PIXEL_MODE_MONO:
			printf("FT_PIXEL_MODE_MONO\n");
			break;
		case FT_PIXEL_MODE_GRAY:
			printf("FT_PIXEL_MODE_GRAY\n");
			break;
		case FT_PIXEL_MODE_GRAY2:
			printf("FT_PIXEL_MODE_GRAY2\n");
			break;
		case FT_PIXEL_MODE_GRAY4:
			printf("FT_PIXEL_MODE_GRAY4\n");
			break;
		case FT_PIXEL_MODE_LCD:
			printf("FT_PIXEL_MODE_LCD\n");
			break;
		case FT_PIXEL_MODE_LCD_V:
			printf("FT_PIXEL_MODE_LCD_V\n");
			break;
		case FT_PIXEL_MODE_BGRA:
			printf("FT_PIXEL_MODE_BGRA\n");
			break;
		}
	}

	bool FontGenerator::ExistFont(wchar_t character, int fontSize) {
		if (this->fontData.find(character) == this->fontData.end()) {
			//���������o��
			return false;
		}
		else if (this->fontData[character].fontData.find(fontSize) == this->fontData[character].fontData.end()) {
			//���̕����̃T�C�Y�����o��
			return false;
		}
		return true;
	}

	void FontGenerator::CreateNewTexture(int fontSize) {
		Texture* texture = new Texture;
		texture->Initialize();
		texture->SetImageData(nullptr, 0, 0, TextureType::Unsigned_Byte, TextureColorType::RED, TextureColorType::RED);

		FontAtlas atlas;
		atlas.texture = texture;
		atlas.buffer = nullptr;
		atlas.width = 0;
		atlas.height = CalclateTextureSize(fontSize + 1);
		atlas.textureWidth = 1;
		atlas.textureHeight = atlas.height;

		this->fontAtlas[fontSize] = atlas;
	}

	void FontGenerator::CreateNewFontAtlas(wchar_t character, int fontSize) {
		//�����e�N�X�`�����쐬
		FT_Set_Pixel_Sizes(this->ftFace, fontSize, fontSize);
		FT_Load_Glyph(this->ftFace, FT_Get_Char_Index(this->ftFace, character), FT_LOAD_RENDER);

		FT_Bitmap bitmap = this->ftFace->glyph->bitmap;
		FontAtlas* atlas = &this->fontAtlas[fontSize];

		//�e�N�X�`���̍����͂��낦��
		char* textBuffer = new char[bitmap.width * atlas->height];
		//�摜�̌������t�ɂȂ�̂ŏC�����Ċi�[
		int count = 0;
		for (int y = bitmap.rows - 1; y >= 0; --y) {
			for (unsigned int x = 0; x < bitmap.width; ++x) {
				textBuffer[count] = bitmap.buffer[y * bitmap.width + x];
				++count;
			}
		}
		//�c���0�ŏ�����
		for (unsigned int i = count; i < bitmap.width * atlas->height; ++i) {
			textBuffer[i] = 0;
		}

		//�����̃f�[�^(����̏ꍇ�͉�)�ɒǉ�
		//�Ċm��
		int newSize = atlas->width + bitmap.width;
		int newTextureSize = atlas->textureWidth;
		while (newTextureSize < newSize) {
			newTextureSize *= 2;
		}
		char* newBuffer = nullptr;
		if (newTextureSize == atlas->textureWidth) {
			//�e�N�X�`���͏\���ɑ傫���A�Ċm�ۂ͕K�v�Ȃ�
			newBuffer = atlas->buffer;
		}
		else {
			//�V�����f�[�^�����邽�߂ɂ̓��������Ċm�ۂ��đ傫������K�v������
			//newBuffer = new char[(atlas->width + bitmap.width) * atlas->height];
			newBuffer = new char[newTextureSize * atlas->height];
			//�R�s�[
			for (unsigned int y = 0; y < atlas->height; ++y) {
				for (unsigned int x = 0; x < atlas->width; ++x) {
					newBuffer[x + y * newTextureSize] = atlas->buffer[y * atlas->textureWidth + x];
				}
			}
			//�T�C�Y���C�����A���Ƃ��Ƃ������摜�z���delete���ĐV�����z��ւ̃|�C���^�ɒu��������
			delete[] atlas->buffer;
			atlas->buffer = newBuffer;
		}
		//�ǉ�
		for (unsigned int y = 0; y < atlas->height; ++y) {
			for (unsigned int x = 0; x < bitmap.width; ++x) {
				atlas->buffer[x + atlas->width + y * newTextureSize] = textBuffer[y * bitmap.width + x];
			}
		}

		//����������������
		FontData data;
		data.locationUV = K_Math::Box2D(atlas->width, atlas->height - bitmap.rows, bitmap.width, bitmap.rows);
		data.fixPositionY = -this->ftFace->glyph->bitmap_top;
		data.fixPositionY += fontSize * this->ftFace->bbox.yMax / (this->ftFace->bbox.yMax - this->ftFace->bbox.yMin);
		data.advance = this->ftFace->glyph->advance.x / 64;
		this->fontData[character].fontData[fontSize] = data;

		atlas->width += bitmap.width;
		atlas->textureWidth = newTextureSize;

		//�e�N�X�`���T�C�Y�ύX & �V�K�摜���i�[
		atlas->texture->SetImageData(atlas->buffer, atlas->textureWidth, atlas->textureHeight, TextureType::Unsigned_Byte, TextureColorType::RED, TextureColorType::RED);
		atlas->texture->SetFilter(true);
		delete[] textBuffer;
	}

	int FontGenerator::CalclateTextureSize(int size) {
		int bit;
		for (bit = 1; bit < size; bit = bit << 1) {
		}
		return bit;
	}

}
