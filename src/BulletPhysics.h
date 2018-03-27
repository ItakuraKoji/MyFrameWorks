#pragma once
#include<bullet\btBulletDynamicsCommon.h>
#include<bullet\BulletCollision\CollisionDispatch\btInternalEdgeUtility.h>
#include<bullet\BulletCollision\CollisionDispatch\btGhostObject.h>
#include"ShaderClass.h"
#include"CameraClass.h"
#include"bulletDebugDraw.h"

#include<iostream>
#include<vector>



//�Փˎ��ɕԂ��^�O���
struct CollisionTag {
	std::string tagName;
	int tagIndex;
	void* userData;
};

//bullet�̃R���W�������������₷�������N���X
class CollisionData {
public:
	CollisionData(btCollisionObject* obj, CollisionTag tag);
	void SetCollisionPosition(btVector3& position);
	btVector3 GetCollisionPosition();

public:
	btCollisionObject* const collision;
	CollisionTag tag;
};

//bullet�̃��[���h���Ǘ�����N���X
class BulletPhysics {
public:

public:
	BulletPhysics();
	~BulletPhysics();
	bool Initialize();
	void Finalize();
	void Run();
	void DebugDraw(ShaderClass* shader, CameraClass* camera, Matrix4f& trans);


	btCollisionShape* CreateTriangleHullShape(btVector3& point1, btVector3& point2, btVector3& point3);
	btTriangleMesh* CreateTriangleMesh(btVector3* vectice, int numFace);
	btCollisionShape* CreateTriangleMeshShape(btTriangleMesh* mesh);
	btCollisionShape* CreateSphereShape(float radius);
	btCollisionShape* CreateCapsuleShape(float radius, float height);
	btCollisionShape* CreateBoxShape(float halfWidth, float halfHeight, float halfDepth);


	//�E���̃I�u�W�F�N�g���쐬���A�|�C���^��Ԃ�
	//�E�`��A���ʁA�ʒu�A��]�̏�
	//�E�Փ˃t�B���^�̃N���[�v�ƃr�b�g�}�X�N
	//�E�ʒu�Ɖ�]�͏ȗ���(�ȗ������ׂĂO)
	CollisionData* CreateRigidBody(btCollisionShape* shape, btScalar mass, int mask, btVector3& pos = btVector3(0, 0, 0), btVector3& rot = btVector3(0, 0, 0));
	//�������Z�ŏՓ˂���R���W�����𐶐��Aghost��true�ɂ���ƕ������Z�͂��̃I�u�W�F�N�g�ɂ͓����Ȃ�
	CollisionData* CreateCollisionObject(btCollisionShape* shape, bool ghost, int mask, btVector3& pos = btVector3(0, 0, 0), btVector3& rot = btVector3(0, 0, 0));

	//�����I�ɐ��E�ɓo�^���Ă��鍄�̂𐢊E����O���Ă���|�C���^��delete
	//���̃N���X�̃f�X�g���N�^�ɂĂ��̊֐��ɂ���đS�ĊJ�����Ă���
	void RemoveCollisionObject(btCollisionObject* rigidbody);

	//�����I�Ƀ��X�g�ɑ��݂���btCollisionShape�����X�g����O����delete
	//���̊֐����Ă΂Ȃ��Ă��A���̃N���X�̃f�X�g���N�^�őS�ĊJ�����Ă���
	void RemoveCollisionShape(btCollisionShape* shape);

	//�E���쐫���ӎ������R���W�����̈ړ��A�ǔ�����s��(�d�������m)
	//����
	//�E�ړ�����R���W�����I�u�W�F�N�g
	//�E�ړ��x�N�g��
	void MoveCharacter(btCollisionObject* obj, btVector3& move);

	//�E���U�I�ȃR���W�����̈ړ��A���肪 MoveCharacter ������G�c(�������y��)
	//����
	//�E�ړ�����R���W�����I�u�W�F�N�g
	//�E�ړ��x�N�g��
	void MoveCharacterDiscrete(btCollisionObject* obj, btVector3& hMove, btVector3& vMove);

	//�E���݂̕������E�ł̓���̃I�u�W�F�N�g�ɑ΂���Փ˂̃`�F�b�N
	//�߂�l
	//�E�Փ˂̗L��
	//����
	//�E���ʂƂ��ĕԂ��Ă���I�u�W�F�N�g��vector
	//�E�Փ˂��`�F�b�N�������I�u�W�F�N�g
	//�E�Փˎ��Ɏg�p����r�b�g�}�X�N�iBulletBitMask�j
	std::vector<CollisionTag>& FindConfrictionObjects(btCollisionObject* myself);


	void SetSkyVector(btVector3& vector);
private:
	//�R���W�������ړ�
	void MoveCollisionObject(btCollisionObject* obj, btVector3& moveVector);
	//�w������Ɉړ��i���U����j
	void MoveDiscrete(btCollisionObject* obj, btVector3& moveVector, bool limitDirection);
	//�w������Ɉړ��iSweep�g�p�j
	btVector3 MoveSmooth(btCollisionObject* obj, btVector3& moveVector, float limitAngle, bool limitDirection);
	//�ړ��������܂Ƃ�
	btVector3 MoveBySweep(btCollisionObject* obj, btVector3& moveVector, bool limitDirection, float allowDistance = 0.2f);
private:
	std::vector<CollisionTag> confrictResult;
	//��ւ̒P�ʃx�N�g���A�����蔻��Ŏg�p
	btVector3 toSkyVector;

	btDiscreteDynamicsWorld*                bulletWorld;
	btDefaultCollisionConfiguration*        config;
	btCollisionDispatcher*                  dispatcher;
	btConstraintSolver*                     solver;
	btBroadphaseInterface*                  broadphase;
	bulletDebugDraw                         debugDrawer;
	btAlignedObjectArray<btCollisionShape*> shapeArray;
};


////���g�ƏՓ˂��Ȃ�sweepTest�̃R�[���o�b�N
struct SweepTestCallBack : public btCollisionWorld::ClosestConvexResultCallback {
public:
	SweepTestCallBack(btCollisionObject *myself);

	//�������g�Ƀq�b�g���Ȃ��悤�ɃI�[�o�[���C�h
	//�߂�l�ɈӖ��͂Ȃ��݂���
	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace);
public:
	btCollisionObject *myself;

};

//�߂荞�ݍő�̖@���x�N�g����������R�[���o�b�N
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
	//�����̍ő�l�Ƃ��̕���
	float maxDistance;
	btVector3 fixVec;

	int count;
	bool isLoop;
};

//���ׂĂ̏Փ˂��L�^����
struct CollectCollisionCallBack : public btCollisionWorld::ContactResultCallback {
public:
	CollectCollisionCallBack(std::vector<CollisionTag>& tagList);
	virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);

public:
	btCollisionObject* obj;
	std::vector<CollisionTag>& result;
	bool isHit;
};

