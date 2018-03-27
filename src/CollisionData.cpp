#include"BulletPhysics.h"

CollisionData::CollisionData(btCollisionObject* obj, CollisionTag tag) : collision(obj), tag(tag) {
}
void CollisionData::SetCollisionPosition(btVector3& position) {
	btTransform trans = this->collision->getWorldTransform();
	trans.setOrigin(position);
	this->collision->setWorldTransform(trans);
}

btVector3 CollisionData::GetCollisionPosition() {
	return this->collision->getWorldTransform().getOrigin();
}