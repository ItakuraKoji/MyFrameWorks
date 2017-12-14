#pragma once

#include<bullet\btBulletDynamicsCommon.h>
#include"ShaderClass.h"
#include"bulletDebugDraw.h"

#include<iostream>
#include<vector>

//bullet�̃��[���h���Ǘ�����N���X
class BulletPhysics {
public:
	BulletPhysics();
	~BulletPhysics();
	bool Initialize();
	void Finalize();
	void Run();
	void DebugDraw(Matrix4f& world, Matrix4f& view, Matrix4f& projection);




	//AddRigidBody()
	//�E���E�ɍ��̂�ǉ�
	//����
	//�E�Z�b�g����I�u�W�F�N�g
	btRigidBody* CreateRigidBody(btCollisionShape* shape, btScalar mass, int mask, btVector3& pos = btVector3(0, 0, 0), btVector3& rot = btVector3(0, 0, 0));

	//AddCollisionObject()
	//�E���E�ɃR���W������ǉ�
	//����
	//�E�Z�b�g����I�u�W�F�N�g
	btCollisionObject* CreateCollisionObject(btCollisionShape* shape, btVector3& pos = btVector3(0, 0, 0), btVector3& rot = btVector3(0, 0, 0));



	void RemoveRigidBody      (btRigidBody* rigidbody);
	void RemoveCollisionObject(btCollisionObject* obj);

	//MoveCharacterObject()
	//�E���쐫���ӎ������R���W�����̈ړ��A�ǔ�����s���i��Ƀv���C���[�p�j
	//�߂�l
	//�E���˂����I�u�W�F�N�g�̖@���i�Փ˂��Ȃ������ꍇ�͒����O��Ԃ��j
	//����
	//�E�ړ����郏�[���h�ɓo�^���ꂽ�I�u�W�F�N�g
	//�E�ړ��x�N�g��
	//�E(�ȗ���)XYZ���A�}���M�����̂悤�ȃM�~�b�N���ł����炢���Ȃ���
	void MoveCharacterObject(btCollisionObject* obj, btVector3& moveVector, btVector3& xAxis = btVector3(1, 0, 0), btVector3& yAxis = btVector3(0, 1, 0), btVector3& zAxis = btVector3(0, 0, 1));


	//FindConfrictionObjects()
	//�E���݂̕������E�ł̓���̃I�u�W�F�N�g�ɑ΂���Փ˂̃`�F�b�N
	//�߂�l
	//�E�Փ˂̗L��
	//����
	//�E���ʂƂ��ĕԂ��Ă���I�u�W�F�N�g��vector
	//�E�Փ˂��`�F�b�N�������I�u�W�F�N�g
	//�E�Փˎ��Ɏg�p����r�b�g�}�X�N�iBulletBitMask�j
	bool FindConfrictionObjects(std::vector<btCollisionObject*>& result, btCollisionObject* myself);

private:
	btVector3 MoveConvexObject(btCollisionObject* obj, btVector3& moveVector, float limitAngle);
	btVector3 MoveSimulation(btCollisionObject* obj, btVector3& moveVector, float& resultHitFraction);
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
	MyConvexClosestCallBack(btCollisionObject *myself) : myself(myself), ClosestConvexResultCallback(btVector3(0, 0, 0), btVector3(0, 0, 0)) {
	}

	//�������g�Ƀq�b�g���Ȃ��悤�ɃI�[�o�[���C�h
	//�߂�l�ɈӖ��͂Ȃ��݂���
	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) {
		if (convexResult.m_hitCollisionObject == this->myself) {
			return btScalar(1.0f);
		}
		ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
		return convexResult.m_hitFraction;
	}
public:
	btCollisionObject *myself;

};

//contactTest�̃R�[���o�b�N��`
struct MyContactCallBack : public btCollisionWorld::ContactResultCallback {
public:
	MyContactCallBack(btCollisionObject* obj) : obj(obj), count(0), isHit(false), ContactResultCallback() {
	}
	//���������I�u�W�F�N�g���L�^
	//�߂�l�ɈӖ��͂Ȃ��݂���
	//�������͎������g�icontactTest�œn�����I�u�W�F�N�g�j�Ƃ͏Փ˂��Ȃ��悤�ł�
	virtual	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {

		//���Փ˔�����o����́A�u�ŋߏՓ˕��Ƃ̋������O�i�덷�܂ށj�v�u�����o�����������[�v�ɓ������i�P�O�O�O�񂮂炢�j�v�Ƃ�
		if (obj == colObj1Wrap->getCollisionObject()) {
			return btScalar(0.0f);
		}
		if (cp.getDistance() >= -0.001f) {
			return btScalar(0.0f);
		}
		if (this->count > 1000) {
			return btScalar(0.0f);
		}

		//�����o��
		btVector3 objPos = obj->getWorldTransform().getOrigin();
		btVector3 normal = cp.m_normalWorldOnB;
		objPos = objPos - cp.m_normalWorldOnB * cp.getDistance() * 0.1f;
		btTransform trans = obj->getWorldTransform();
		trans.setOrigin(objPos);
		obj->setWorldTransform(trans);

		this->isHit = true;
		++this->count;
		return btScalar(0.0f);
	}

public:
	//�����o���I�u�W�F�N�g
	btCollisionObject* obj;
	//�������[�v�h�~
	int count;
	//����
	bool isHit;
};

