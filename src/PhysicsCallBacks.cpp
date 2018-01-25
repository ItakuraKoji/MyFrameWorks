#include"BulletPhysics.h"

SweepTestCallBack::SweepTestCallBack(btCollisionObject *myself) : myself(myself), ClosestConvexResultCallback(btVector3(0, 0, 0), btVector3(0, 0, 0)) {
}

//���g�ƏՓ˂��Ȃ�sweepTest�̃R�[���o�b�N
btScalar SweepTestCallBack::addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) {
	if (convexResult.m_hitCollisionObject == this->myself) {
		return btScalar(1.0f);
	}
	ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
	return convexResult.m_hitFraction;
}


FixContactCallBack::FixContactCallBack(btCollisionObject* obj) : obj(obj), count(0), isHit(false), ContactResultCallback() {
}

//�Փ˂����������܂Ŗ@�������ɕ��̂������o���R�[���o�b�N
btScalar FixContactCallBack::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {

	//���Փ˔�����o����́A�u�ŋߏՓ˕��Ƃ̋������O�i�덷�܂ށj�v�u�����o�����������[�v�ɓ������i�P�O�O�O�񂮂炢�j�v�Ƃ�
	if (obj == colObj1Wrap->getCollisionObject()) {
		return btScalar(0.0f);
	}
	if (cp.getDistance() >= -0.001f) {
		return btScalar(0.0f);
	}
	if (this->count > 500) {
		return btScalar(0.0f);
	}

	//�����o��
	btVector3 objPos = obj->getWorldTransform().getOrigin();
	btVector3 normal = cp.m_normalWorldOnB;
	objPos = objPos - cp.m_normalWorldOnB * cp.getDistance() * 0.4f;
	btTransform trans = obj->getWorldTransform();
	trans.setOrigin(objPos);
	obj->setWorldTransform(trans);

	this->isHit = true;
	++this->count;
	return btScalar(0.0f);
}