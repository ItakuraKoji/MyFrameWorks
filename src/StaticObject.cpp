#include"StaticObject.h"

bool StaticObject::Initialize(GameParameters* param) {
	btCollisionShape *characterShape = param->GetPhysics()->CreateCapsuleShape(0.3f, 0.8f);
	return true;
}
void StaticObject::Finalize() {
	delete this->mapCollisions;
}
void StaticObject::Run(GameParameters* param) {
//‰½‚à‚µ‚È‚¢
}

void StaticObject::Draw(GameParameters* param) {
	this->GetModel()->Draw(param, this->position, this->rotation, this->scale);
}

void StaticObject::SetMapCollision(MapPolygon* map) {
	this->mapCollisions = map;
}

