#pragma once
#include"FontGenerator.h"
#include"SpriteObject.h"
#include"GameParameters.h"

//�t�H���g�𐶐����A�`�悷��N���X(���C�h������)
class FontRenderer {
public:
	FontRenderer(const char* text);
	~FontRenderer();
	//�w����W����e�L�X�g��`��
	void DrawString(GameParameters* param, const std::string& text, int size, int screenX, int screenY);
	//�t�H���g�`��ɗ��p���Ă���e�N�X�`�����폜���ă����������(�f�X�g���N�^�ōs���Ă���)
	void ClearFontTexture();

private:


private:
	FontGenerator* generator;
	SpriteObject* sprite;
};