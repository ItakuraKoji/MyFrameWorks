#pragma once
#include"FontGenerator.h"
#include"SpriteObject.h"
#include"GameParameters.h"

//フォントを生成し、描画するクラス(ワイド文字列)
class FontRenderer {
public:
	FontRenderer(const char* text);
	~FontRenderer();
	//指定座標からテキストを描画
	void DrawString(GameParameters* param, const std::string& text, int size, int screenX, int screenY);
	//フォント描画に利用しているテクスチャを削除してメモリを解放(デストラクタで行っている)
	void ClearFontTexture();

private:


private:
	FontGenerator* generator;
	SpriteObject* sprite;
};