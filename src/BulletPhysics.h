#pragma once

#include<bullet\btBulletDynamicsCommon.h>
#include"ShaderClass.h"
#include"bulletDebugDraw.h"

#include<iostream>
#include<vector>

enum BulletBitMask
{
	BIT_ZERO = 0,
	BIT_MAP = 1,
	BIT_CHARACTER = 2,
};

//bullet�̃��[���h���Ǘ�����N���X
class BulletPhysics {
public:
	BulletPhysics();
	~BulletPhysics();
	bool Initialize();
	void Run();
	void DebugDraw(Matrix4f& world, Matrix4f& view, Matrix4f& projection);

	//AddRigidBody()
	//�E���E�ɍ��̂�ǉ�
	//����
	//�E�Z�b�g����I�u�W�F�N�g
	//�E�ړ����ɏՓ˂��邩�̃r�b�g�t���O
	//�E���蔲���邯�ǏՓ˔���͂���镨�̂̃t���O
	void AddRigidBody         (btRigidBody* rigidbody, int confrictMask, int hitMask);
	void AddCollisionObject   (btCollisionObject* obj);
	void RemoveRigidBody      (btRigidBody* rigidbody);
	void RemoveCollisionObject(btCollisionObject* obj);

	//MoveCharacterObject()
	//�E���쐫���ӎ������R���W�����̈ړ��A�ǔ�����s���i�ǂ��萶�����֎~���邱�ƂŒP���Ȉړ�������\�j
	//�߂�l
	//�E���˂����I�u�W�F�N�g�̖@���i�Փ˂��Ȃ������ꍇ�͒����O��Ԃ��j
	//����
	//�E�ړ����郏�[���h�ɓo�^���ꂽ�I�u�W�F�N�g
	//�E�ړ��x�N�g��
	//�E���񒣂�̗����i�ǂ�������Ȃ��j���E�̊p�x�@���P�����x�̌덷����
	btVector3 MoveCharacterObject(btCollisionObject* obj, btVector3 &moveVector, float limitAngle);
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
		if (!(convexResult.m_hitCollisionObject->getUserIndex() & myself->getUserIndex())) {
			return btScalar(1.0f);
		}
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
	MyContactCallBack(btCollisionObject* obj) : obj(obj), isHit(false), ContactResultCallback() {
	}
	//���������I�u�W�F�N�g���L�^
	//�߂�l�ɈӖ��͂Ȃ��݂���
	//�������͎������g�icontactTest�œn�����I�u�W�F�N�g�j�Ƃ͏Փ˂��Ȃ��悤�ł�
	virtual	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
		//�r�b�g�}�X�N���m�F
		//if (!(colObj1Wrap->getCollisionObject()->getUserIndex2() & colObj0Wrap->getCollisionObject()->getUserIndex2())) {
		//	return btScalar(0.0f);
		//}

		if (obj == colObj1Wrap->getCollisionObject()) {
			return btScalar(0.0f);
		}

		if (cp.getDistance() >= -0.001f) {
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
		return btScalar(0.0f);
	}

public:
	//�����o���I�u�W�F�N�g
	btCollisionObject* obj;

	bool isHit;
};

