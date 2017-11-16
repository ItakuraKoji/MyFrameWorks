#pragma once

#include<bullet\btBulletDynamicsCommon.h>
#include"ShaderClass.h"
#include"bulletDebugDraw.h"
#include<iostream>

//bullet�̃��[���h���Ǘ�����N���X
class BulletPhysics {
public:
	BulletPhysics();
	~BulletPhysics();
	bool Initialize();
	void Run();
	void DebugDraw(float *world, float *view, float *projection);

	void AddRigidBody         (btRigidBody *rigidbody);
	void AddCollisionObject   (btCollisionObject *obj);
	void RemoveRigidBody      (btRigidBody *rigidbody);
	void RemoveCollisionObject(btCollisionObject *obj);

	bool MoveCharacterObject(btCollisionObject *obj, btVector3 &moveVector);

private:
	ShaderClass *shader;
	btDiscreteDynamicsWorld*                bulletWorld;
	btDefaultCollisionConfiguration*        config;
	btCollisionDispatcher*                  dispatcher;
	btConstraintSolver*                     solver;
	btBroadphaseInterface*                  broadphase;
	btAlignedObjectArray<btCollisionShape*> shapeArray;
	bulletDebugDraw                         debugDrawer;
};

//sweepTest�̃R�[���o�b�N���`
struct MyConvexClosestCallBack : public btCollisionWorld::ClosestConvexResultCallback {
public:
	MyConvexClosestCallBack(btCollisionObject *myself) : myself(myself), ClosestConvexResultCallback(btVector3(0, 0, 0), btVector3(0, 0, 0)) {}

	//�������g�Ƀq�b�g���Ȃ��悤�ɃI�[�o�[���C�h
	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) {
		if (convexResult.m_hitCollisionObject == this->myself) {
			return btScalar(1.0f);
		}
		return ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
	}
public:
	btCollisionObject *myself;
};