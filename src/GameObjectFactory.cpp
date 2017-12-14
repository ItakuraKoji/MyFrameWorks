#include"GameObjectFactory.h"

GameObject* GameObjectFactory::Create(const std::string& name, GameParameters& param) {
	return CreateGameObject(name, param);
}