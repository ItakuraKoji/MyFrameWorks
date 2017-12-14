#pragma once

#include<bullet\btBulletDynamicsCommon.h>
#include"ShaderClass.h"
#include"bulletDebugDraw.h"

#include<iostream>
#include<vector>

//bulletのワールドを管理するクラス
class BulletPhysics {
public:
	BulletPhysics();
	~BulletPhysics();
	bool Initialize();
	void Finalize();
	void Run();
	void DebugDraw(Matrix4f& world, Matrix4f& view, Matrix4f& projection);




	//AddRigidBody()
	//・世界に剛体を追加
	//引数
	//・セットするオブジェクト
	btRigidBody* CreateRigidBody(btCollisionShape* shape, btScalar mass, int mask, btVector3& pos = btVector3(0, 0, 0), btVector3& rot = btVector3(0, 0, 0));

	//AddCollisionObject()
	//・世界にコリジョンを追加
	//引数
	//・セットするオブジェクト
	btCollisionObject* CreateCollisionObject(btCollisionShape* shape, btVector3& pos = btVector3(0, 0, 0), btVector3& rot = btVector3(0, 0, 0));



	void RemoveRigidBody      (btRigidBody* rigidbody);
	void RemoveCollisionObject(btCollisionObject* obj);

	//MoveCharacterObject()
	//・操作性を意識したコリジョンの移動、壁判定も行う（主にプレイヤー用）
	//戻り値
	//・激突したオブジェクトの法線（衝突しなかった場合は長さ０を返す）
	//引数
	//・移動するワールドに登録されたオブジェクト
	//・移動ベクトル
	//・(省略可)XYZ軸、マリギャラのようなギミックができたらいいなって
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
	MyContactCallBack(btCollisionObject* obj) : obj(obj), count(0), isHit(false), ContactResultCallback() {
	}
	//当たったオブジェクトを記録
	//戻り値に意味はないみたい
	//こっちは自分自身（contactTestで渡したオブジェクト）とは衝突しないようです
	virtual	btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {

		//未衝突判定を出す基準は、「最近衝突物との距離が０（誤差含む）」「押し出しが無限ループに入った（１０００回ぐらい）」とき
		if (obj == colObj1Wrap->getCollisionObject()) {
			return btScalar(0.0f);
		}
		if (cp.getDistance() >= -0.001f) {
			return btScalar(0.0f);
		}
		if (this->count > 1000) {
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
		++this->count;
		return btScalar(0.0f);
	}

public:
	//押し出すオブジェクト
	btCollisionObject* obj;
	//無限ループ防止
	int count;
	//結果
	bool isHit;
};

