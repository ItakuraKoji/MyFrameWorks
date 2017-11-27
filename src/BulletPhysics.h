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

//bulletのワールドを管理するクラス
class BulletPhysics {
public:
	BulletPhysics();
	~BulletPhysics();
	bool Initialize();
	void Run();
	void DebugDraw(Matrix4f& world, Matrix4f& view, Matrix4f& projection);

	//AddRigidBody()
	//・世界に剛体を追加
	//引数
	//・セットするオブジェクト
	//・移動時に衝突するかのビットフラグ
	//・すり抜けるけど衝突判定はされる物体のフラグ
	void AddRigidBody         (btRigidBody* rigidbody, int confrictMask, int hitMask);
	void AddCollisionObject   (btCollisionObject* obj);
	void RemoveRigidBody      (btRigidBody* rigidbody);
	void RemoveCollisionObject(btCollisionObject* obj);

	//MoveCharacterObject()
	//・操作性を意識したコリジョンの移動、壁判定も行う（壁ずり生成を禁止することで単純な移動判定も可能）
	//戻り値
	//・激突したオブジェクトの法線（衝突しなかった場合は長さ０を返す）
	//引数
	//・移動するワールドに登録されたオブジェクト
	//・移動ベクトル
	//・踏ん張りの利く（壁ずりを作らない）限界の角度　※１°程度の誤差あり
	btVector3 MoveCharacterObject(btCollisionObject* obj, btVector3 &moveVector, float limitAngle);
	void MoveCharacterObject(btCollisionObject* obj, btVector3& moveVector, btVector3& xAxis = btVector3(1, 0, 0), btVector3& yAxis = btVector3(0, 1, 0), btVector3& zAxis = btVector3(0, 0, 1));


	//FindConfrictionObjects()
	//・現在の物理世界での特定のオブジェクトに対する衝突のチェック
	//戻り値
	//・衝突の有無
	//引数
	//・結果として返ってくるオブジェクトのvector
	//・衝突をチェックしたいオブジェクト
	//・衝突時に使用するビットマスク（BulletBitMask）
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

//sweepTestのコールバックを定義
struct MyConvexClosestCallBack : public btCollisionWorld::ClosestConvexResultCallback {
public:
	MyConvexClosestCallBack(btCollisionObject *myself) : myself(myself), ClosestConvexResultCallback(btVector3(0, 0, 0), btVector3(0, 0, 0)) {
	}

	//自分自身にヒットしないようにオーバーライド
	//戻り値に意味はないみたい
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

//contactTestのコールバック定義
struct MyContactCallBack : public btCollisionWorld::ContactResultCallback {
public:
	MyContactCallBack(btCollisionObject* obj) : obj(obj), isHit(false), ContactResultCallback() {
	}
	//当たったオブジェクトを記録
	//戻り値に意味はないみたい
	//こっちは自分自身（contactTestで渡したオブジェクト）とは衝突しないようです
	virtual	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {
		//ビットマスクを確認
		//if (!(colObj1Wrap->getCollisionObject()->getUserIndex2() & colObj0Wrap->getCollisionObject()->getUserIndex2())) {
		//	return btScalar(0.0f);
		//}

		if (obj == colObj1Wrap->getCollisionObject()) {
			return btScalar(0.0f);
		}

		if (cp.getDistance() >= -0.001f) {
			return btScalar(0.0f);
		}

		//押し出し
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
	//押し出すオブジェクト
	btCollisionObject* obj;

	bool isHit;
};

