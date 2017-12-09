#include"GameObjectFactory.h"

GameObject* GameObjectFactory::Create(const std::string& name, DrawParameters& param) {
	return CreateGameObject(name, param);
}