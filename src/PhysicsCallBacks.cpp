#include"BulletPhysics.h"

namespace K_Physics {

	////////
	//public
	////

	SweepTestCallBack::SweepTestCallBack(btCollisionObject *myself) : myself(myself), ClosestConvexResultCallback(btVector3(0, 0, 0), btVector3(0, 0, 0)) {
	}

	//���g�ƏՓ˂��Ȃ�sweepTest�̃R�[���o�b�N
	btScalar SweepTestCallBack::addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) {
		if (this->myself->getCollisionFlags() && btCollisionObject::CF_NO_CONTACT_RESPONSE) {
			return btScalar(1.0f);
		}
		if (convexResult.m_hitCollisionObject == this->myself) {
			return btScalar(1.0f);
		}
		ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
		return convexResult.m_hitFraction;
	}


	FixContactCallBack::FixContactCallBack(btCollisionObject* obj) : obj(obj), count(0), isLoop(false), ContactResultCallback() {
		this->maxDistance = 0.0f;
		this->fixVec = btVector3(0.0f, 0.0f, 0.0f);
		this->obj = obj;
	}

	//�߂荞�ݍő�̖@���x�N�g����������R�[���o�b�N
	btScalar FixContactCallBack::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
		//�u�ŋߏՓ˕��Ƃ̋������O�i�덷�܂ށj�v�u���[�v�����񐔂ɒB�����v�Ƃ����[�v�𔲂���
		this->isLoop = false;
		if (this->obj->getCollisionFlags() && btCollisionObject::CF_NO_CONTACT_RESPONSE) {
			return btScalar(0.0f);
		}
		if (this->obj == colObj1Wrap->getCollisionObject()) {
			return btScalar(0.0f);
		}
		if (cp.getDistance() >= -0.01f) {
			return btScalar(0.0f);
		}
		if (this->count > 5) {
			return btScalar(0.0f);
		}

		//�ő�l�X�V
		if (this->maxDistance > cp.getDistance()) {
			this->maxDistance = cp.getDistance();
			this->fixVec = cp.m_normalWorldOnB;
		}
		++this->count;
		this->isLoop = true;
		return btScalar(0.0f);
	}

	CollectCollisionCallBack::CollectCollisionCallBack(std::vector<CollisionTag>& tagList) : result(tagList), isHit(false) {
		tagList.clear();
	}
	btScalar CollectCollisionCallBack::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
		//�Փ˂���������L�^
		CollisionData* data = (CollisionData*)colObj1Wrap->m_collisionObject->getUserPointer();
		if (data) {
			this->result.push_back(data->tag);
			this->isHit = true;
		}
		return 0;
	}
}
