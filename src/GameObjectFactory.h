#pragma once

#include<string>
#include"DrawParameters.h"
#include"GameObject.h"


enum ModelType {
	Skin,
	Static,
	Square,
};

//GameObjectの生成担当クラス、派生クラスにて実装
class GameObjectFactory {
public:
	GameObject* Create(const std::string& name, GameParameters& param);
protected:
	//具体的な実装はここ
	virtual GameObject* CreateGameObject(const std::string& name, GameParameters& param) = 0;
};


//以下派生クラス
