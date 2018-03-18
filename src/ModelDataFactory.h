#pragma once
#include<string>
#include"TextureList.h"
#include"ModelData.h"
#include"FbxModelLoader.h"

//モデルクラスの初期化に必要なパラメーターの製作を担当するクラス
//・外部ファイルの読み込み
//・基本図形(板ポリゴンなど)
class ModelDataFactory {
public:
	ModelDatas* LoadFBXModel(const std::string& fileName, TextureList* textureList);
	ModelDatas* CreateSquareModel(float width, float height, Texture* texture, bool isCenter = true);
};