#include"StaticObject.h"

bool StaticObject::Initialize(GameParameters* param) {
	//btCollisionShape *characterShape = param->GetPhysics()->CreateCapsuleShape(0.3f, 0.8f);
	return true;
}
void StaticObject::Finalize() {
	delete this->mapCollisions;
}
void StaticObject::Run(GameParameters* param) {
//何もしない
}

void StaticObject::Draw(GameParameters* param) {
	this->GetModel()->Draw(param->currentCamera, param->currentShader, this->position, this->rotation, this->scale);
}

void StaticObject::SetMapCollision(K_Physics::MapPolygon* map) {
	this->mapCollisions = map;
}

