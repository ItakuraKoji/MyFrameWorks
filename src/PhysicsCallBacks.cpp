#include"BulletPhysics.h"

SweepTestCallBack::SweepTestCallBack(btCollisionObject *myself) : myself(myself), ClosestConvexResultCallback(btVector3(0, 0, 0), btVector3(0, 0, 0)) {
}

//自身と衝突しないsweepTestのコールバック
btScalar SweepTestCallBack::addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) {
	if (convexResult.m_hitCollisionObject == this->myself) {
		return btScalar(1.0f);
	}
	ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
	return convexResult.m_hitFraction;
}


FixContactCallBack::FixContactCallBack(btCollisionObject* obj) : obj(obj), count(0), isHit(false), ContactResultCallback() {
}

//衝突が解消されるまで法線方向に物体を押し出すコールバック
btScalar FixContactCallBack::addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {

	//未衝突判定を出す基準は、「最近衝突物との距離が０（誤差含む）」「押し出しが無限ループに入った（１０００回ぐらい）」とき
	if (obj == colObj1Wrap->getCollisionObject()) {
		return btScalar(0.0f);
	}
	if (cp.getDistance() >= -0.001f) {
		return btScalar(0.0f);
	}
	if (this->count > 500) {
		return btScalar(0.0f);
	}

	//押し出し
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