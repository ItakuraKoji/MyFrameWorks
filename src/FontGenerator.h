#pragma once

#include<unordered_map>
#include<FreeType\ft2build.h>
#include FT_FREETYPE_H

#include"Texture.h"
#include"MyMathFanctions.h"


//�t�H���g�A�g���X���쐬�E�ێ��E�X�V����N���X
//��Ƀt�H���g�`��N���X�Ɏg���Ă��炤
class FontGenerator {
public:
	//�t�H���g�e�N�X�`���̏��
	struct FontAtlas {
		Texture* texture;
		char*    buffer;
		unsigned int width;
		unsigned int height;
		unsigned int textureWidth;
		unsigned int textureHeight;
	};
	struct FontData {
		//Texture UV by Pixel Unit
		M::Box2D locationUV;
		//Rendering Position("Y" direction)
		int fixPositionY;
		//next Font Position("X" direction)
		int advance;
	};

	//�T�C�Y�͏c�Ɖ��œ������̂����󂯕t���Ȃ�
	//�c���Ƃ������Ƃ��̓X�P�[���������Ċe���ł���Ă�������
	struct CharData {
		// key   : fontSize
		// value : fontAtlas UV
		std::unordered_map<int, FontData> fontData;
	};

public:
	FontGenerator(const char* fontFilePass);
	~FontGenerator();

	//���łɕ��������݂��Ă��邩�𒲂ׂāA�V���������Ȃ�ǉ�
	void SetFont(wchar_t character, int fontSize);

	//�e�N�X�`�����擾
	Texture* GetFontAtlas(int fontSize);
	//�w��̕���UV���擾
	//�f�[�^���Ȃ�������false�Ԃ��̏�ŎQ�Ƃɂ͈�؂̕ω��Ȃ��i�V�K�쐬�͈������ɔC����j
	bool GetFontData(FontData& result, wchar_t font, int fontSize);

	//�w�肵���t�H���g�T�C�Y�̃e�N�X�`������ɂ���
	void ClearFontAtlas(int fontSize);
	//�S�Ẵt�H���g�A�g���X���폜
	void ClearAllFontAtlas();

private:
	void ShowPixelType(FT_Bitmap bitmap);
	bool ExistFont(wchar_t character, int fontSize);
	void CreateNewTexture(int fontSize);
	void CreateNewFontAtlas(wchar_t character, int fontSize);
	//2�̗ݏ�l�œK�؂ȃe�N�X�`���T�C�Y���Z�o���ĕԂ�
	int CalclateTextureSize(int size);

private:
	// key   : fontSize
	// value : Pointer to TextureClass and ImageData Array
	std::unordered_map<int, FontAtlas> fontAtlas;
	// key   : character
	// value : character's fontSize And fontAtlas UVs
	std::unordered_map<wchar_t, CharData> fontData;

	//FreeType
	FT_Library ftLib;
	FT_Face ftFace;
};