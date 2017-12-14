#pragma once

#include<string>
#include"DrawParameters.h"
#include"GameObject.h"


enum ModelType {
	Skin,
	Static,
	Square,
};

//GameObject�̐����S���N���X�A�h���N���X�ɂĎ���
class GameObjectFactory {
public:
	GameObject* Create(const std::string& name, GameParameters& param);
protected:
	//��̓I�Ȏ����͂���
	virtual GameObject* CreateGameObject(const std::string& name, GameParameters& param) = 0;
};


//�ȉ��h���N���X
