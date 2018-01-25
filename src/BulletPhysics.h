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
	void DebugDraw(ShaderClass* shader, Matrix4f& world, Matrix4f& view, Matrix4f& projection);


	btCollisionShape* CreateTriangleShape(btVector3& point1, btVector3& point2, btVector3& point3);
	btCollisionShape* CreateSphereShape(float radius);
	btCollisionShape* CreateCapsuleShape(float radius, float height);
	btCollisionShape* CreateBoxShape(float halfWidth, float halfHeight, float halfDepth);


	//CreateRigidBody()
	//�E���̃I�u�W�F�N�g���쐬���A�|�C���^��Ԃ�
	//�E�`��A���ʁA�ʒu�A��]�̏�
	//�E�Փ˃t�B���^�̃N���[�v�ƃr�b�g�}�X�N
	//�E�ʒu�Ɖ�]�͏ȗ���(�ȗ������ׂĂO)
	btRigidBody* CreateRigidBody(btCollisionShape* shape, btScalar mass, int group, int mask, btVector3& pos = btVector3(0, 0, 0), btVector3& rot = btVector3(0, 0, 0));

	//�����I�ɐ��E�ɓo�^���Ă��鍄�̂𐢊E����O���Ă���|�C���^��delete
	//���̊֐����Ă΂Ȃ��Ă��A���̃N���X�̃f�X�g���N�^�őS�ĊJ�����Ă���
	void RemoveRigidBody(btRigidBody* rigidbody);

	//�����I�Ƀ��X�g�ɑ��݂���btCollisionShape�����X�g����O����delete
	//���̊֐����Ă΂Ȃ��Ă��A���̃N���X�̃f�X�g���N�^�őS�ĊJ�����Ă���
	void RemoveCollisionShape(btCollisionShape* shape);

	//MoveCharacterObject()
	//�E���쐫���ӎ������R���W�����̈ړ��A�ǔ�����s���i��Ƀv���C���[�p�j
	//����
	//�E�ړ�����R���W�����I�u�W�F�N�g
	//�E�ړ��x�N�g��
	void MoveCharacterObject(btCollisionObject* obj, btVector3& hMove, btVector3& vMove);

	//DiscreteMoveObject()
	//�E���U�I�ȃR���W�����̈ړ��A���肪�v���C���[�p������G�c�i���NPC�p�j
	//����
	//�E�ړ�����R���W�����I�u�W�F�N�g
	//�E�ړ��x�N�g��
	void DiscreteMoveObject(btCollisionObject* obj, btVector3& hMove, btVector3& vMove);


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
	btVector3 MoveSimulation(btCollisionObject* obj, btVector3& moveVector);
private:
	btDiscreteDynamicsWorld*                bulletWorld;
	btDefaultCollisionConfiguration*        config;
	btCollisionDispatcher*                  dispatcher;
	btConstraintSolver*                     solver;
	btBroadphaseInterface*                  broadphase;
	bulletDebugDraw                         debugDrawer;
	btAlignedObjectArray<btCollisionShape*> shapeArray;
};


//sweepTest�̃R�[���o�b�N���`
struct SweepTestCallBack : public btCollisionWorld::ClosestConvexResultCallback {
public:
	SweepTestCallBack(btCollisionObject *myself);

	//�������g�Ƀq�b�g���Ȃ��悤�ɃI�[�o�[���C�h
	//�߂�l�ɈӖ��͂Ȃ��݂���
	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace);
public:
	btCollisionObject *myself;

};

//contactTest�̃R�[���o�b�N��`
struct FixContactCallBack : public btCollisionWorld::ContactResultCallback {
public:
	FixContactCallBack(btCollisionObject* obj);
	//���������I�u�W�F�N�g���L�^
	//�߂�l�ɈӖ��͂Ȃ��݂���
	//�������͎������g�icontactTest�œn�����I�u�W�F�N�g�j�Ƃ͏Փ˂��Ȃ��悤�ł�
	virtual	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);

public:
	//�����o���I�u�W�F�N�g
	btCollisionObject* obj;
	//�������[�v�h�~
	int count;
	//����
	bool isHit;
};

