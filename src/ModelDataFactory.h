#pragma once
#include<string>
#include"DrawParameters.h"
#include"ModelData.h"
#include"FbxModelLoader.h"

//モデルクラスの初期化に必要なパラメーターの製作を担当するクラス
//・外部ファイルの読み込み
//・基本図形(板ポリゴンなど)
class ModelDataFactory {
public:
	ModelDatas* LoadFBXModel(const std::string& fileName, DrawParameters& param);
	ModelDatas* CreateSquareModel(const std::string& textureName, DrawParameters& param);
};