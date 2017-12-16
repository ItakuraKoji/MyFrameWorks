#include"BulletPhysics.h"

BulletPhysics::BulletPhysics() {
	Initialize();
}
BulletPhysics::~BulletPhysics() {
	Finalize();
}

//bulletWorldの初期化
bool BulletPhysics::Initialize() {
	this->config      = new btDefaultCollisionConfiguration;
	this->dispatcher  = new btCollisionDispatcher(this->config);
	this->broadphase  = new btDbvtBroadphase;
	this->solver      = new btSequentialImpulseConstraintSolver;
	this->bulletWorld = new btDiscreteDynamicsWorld(this->dispatcher, this->broadphase, this->solver, this->config);
	
	this->debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints);
	this->bulletWorld->setDebugDrawer(&this->debugDrawer);

	this->bulletWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));

	try {
		this->shader = new ShaderClass("Shader/SimpleShader.vs", "Shader/SimpleShader.ps");
	}
	catch(...){
		//何もしない
	}
	debugDrawer.SetShader(this->shader);
	return true;
}
//開放
void BulletPhysics::Finalize() {
	for (int i = this->bulletWorld->getNumCollisionObjects() - 1; i >= 0; --i) {
		btCollisionObject* obj = this->bulletWorld->getCollisionObjectArray()[i];
		btRigidBody*       rigid = btRigidBody::upcast(obj);
		if (rigid && rigid->getMotionState()) {
			delete rigid->getMotionState();
		}
		this->bulletWorld->removeCollisionObject(obj);
		delete obj;
	}
	for (int i = 0; i < this->shapeArray.size(); ++i) {
		delete this->shapeArray[i];
	}

	delete this->bulletWorld;
	delete this->solver;
	delete this->broadphase;
	delete this->dispatcher;
	delete this->config;
	delete this->shader;
}

//シミュレーションを進める
void BulletPhysics::Run() {
	this->bulletWorld->stepSimulation(1 / 60.0f, 10, 1 / 60.0f);
}
//デバッグ描画
void BulletPhysics::DebugDraw(Matrix4f& world, Matrix4f& view, Matrix4f& projection) {
	//シェーダーの読み込みに失敗したときはデバッグ描画できない
	if (!this->shader) {
		return;
	}

	this->shader->UseShader();

	Matrix4f mat = projection * view * world;
	this->shader->SetMatrix(mat);
	this->bulletWorld->debugDrawWorld();
}

//オブジェクト生成
//三角形
btCollisionShape* BulletPhysics::CreateTriangleShape(btVector3& point1, btVector3& point2, btVector3& point3) {
	btScalar points[9] = { point1.x(),point1.y(),point1.z(),
		                   point2.x(),point2.y(),point2.z(),
		                   point3.x(),point3.y(),point3.z() };
	btCollisionShape* shape = new btConvexHullShape(points, 3, 3 * sizeof(btScalar));
	this->shapeArray.push_back(shape);
	return shape;
}
//球
btCollisionShape* BulletPhysics::CreateSphereShape(float radius) {
	btCollisionShape* shape = new btSphereShape(radius);
	this->shapeArray.push_back(shape);
	return shape;
}
//カプセル
btCollisionShape* BulletPhysics::CreateCapsuleShape(float radius, float height) {
	btCollisionShape* shape = new btCapsuleShape(radius, height);
	this->shapeArray.push_back(shape);
	return shape;
}
//長方形
btCollisionShape* BulletPhysics::CreateBoxShape(float halfWidth, float halfHeight, float halfDepth) {
	btCollisionShape* shape = new btBoxShape(btVector3(halfWidth, halfHeight, halfDepth));
	this->shapeArray.push_back(shape);
	return shape;
}

//剛体
btRigidBody* BulletPhysics::CreateRigidBody(btCollisionShape* shape, btScalar mass, int group, int mask, btVector3& pos, btVector3& rot) {
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(pos);
	btQuaternion q = btQuaternion(btVector3(0, 0, 1), rot.z()) * btQuaternion(btVector3(1, 0, 0), rot.x()) * btQuaternion(btVector3(0, 1, 0), rot.y());
	trans.setRotation(q);

	btVector3 inertia(0.0f, 0.0f, 0.0f);
	if (mass > 0.0f) {
		shape->calculateLocalInertia(mass, inertia);
	}
	btDefaultMotionState *state = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo info(mass, state, shape, inertia);
	btRigidBody *rigid = new btRigidBody(info);

	this->bulletWorld->addRigidBody(rigid, group, mask);
	return rigid;
}

//開放
void BulletPhysics::RemoveRigidBody(btRigidBody* obj) {
	this->bulletWorld->removeRigidBody(obj);
	delete obj;
	obj = 0;
}
void BulletPhysics::RemoveCollisionShape(btCollisionShape* shape) {
	this->shapeArray.remove(shape);
	delete shape;
	shape = 0;
}


bool BulletPhysics::FindConfrictionObjects(std::vector<btCollisionObject*>& resultArray, btCollisionObject* myself) {
	return true;
}

//新型衝突シミュレーション
void BulletPhysics::MoveCharacterObject(btCollisionObject *obj, btVector3& hMove, btVector3& vMove) {
	//凹形状とは判定できない
	if (!obj->getCollisionShape()->isConvex()) {
		return;
	}

	//移動は縦と横で分離
	btVector3 normal;
	btTransform prevTrans = obj->getWorldTransform();
	//まず法線を取得してみる
	normal = MoveConvexObject(obj, vMove, 40.0f);

	if (normal.norm() >= 0.001f) {
		//床基準の方向軸
		if (normal.dot(vMove) > 0) {
			vMove = vMove.norm() * normal;
		}
		else if (normal.dot(vMove) < 0) {
			vMove = -vMove.norm() * normal;
		}

	}
	obj->setWorldTransform(prevTrans);
	//縦
	btVector3 virtical = vMove;
	MoveConvexObject(obj, virtical, 40.0f);

	//横
	//btVector3 horizontal = (moveVector.x()) * xAxis + (moveVector.z()) * zAxis;
	MoveConvexObject(obj, hMove, 0.0f);

}

void BulletPhysics::DiscreteMoveObject(btCollisionObject *obj, btVector3& hMove, btVector3& vMove) {
	//凹形状とは判定できない
	if (!obj->getCollisionShape()->isConvex()) {
		return;
	}
	//移動
	btTransform to = obj->getWorldTransform();
	to.setOrigin(to.getOrigin() + hMove + vMove);
	obj->setWorldTransform(to);

	//衝突が解消されるまで法線方向に押し出す
	MyContactCallBack contact_cb(obj);
	do {
		contact_cb.isHit = false;
		this->bulletWorld->contactTest(obj, contact_cb);
	} while (contact_cb.isHit);

}


////////
//private
////

//指定方向に移動（壁ずり付き）
btVector3 BulletPhysics::MoveConvexObject(btCollisionObject *obj, btVector3 &moveVector, float limitAngle) {
	if (moveVector.norm() < 0.001f) {
		return btVector3(0, 0, 0);
	}

	float hitFraction = 0.0f;
	btVector3 normal;


	//移動一回目
	normal = MoveSimulation(obj, moveVector, hitFraction);


	//壁ずりを作る角度かを確認
	float angle_cos = (float)-normal.dot(moveVector.normalized());
	float limit_cos = (float)btCos(btRadians(limitAngle));
	if (abs(angle_cos - limit_cos) < 0.0001f) {
		angle_cos = limit_cos;
	}
	if (angle_cos > limit_cos || normal.norm() < 0.001f) {
		return normal;
	}

	//壁ずりを作る
	btVector3 goVec;
	goVec.setZero();
	goVec = moveVector * (1.0f - hitFraction);
	goVec += normal * (-goVec.dot(normal));

	if (goVec.norm() < 0.001f) {
		return btVector3(0, 0, 0);
	}

	//移動二回目
	MoveSimulation(obj, goVec, hitFraction);


	//壁ずり時は衝突法線を返さない
	return btVector3(0, 0, 0);
}

//移動部分をまとめ
btVector3 BulletPhysics::MoveSimulation(btCollisionObject *obj, btVector3 &moveVector, float &resultHitFraction){
	const float allowDistance = 0.01f;
	btConvexShape* shape = (btConvexShape*)obj->getCollisionShape();

	btTransform from = obj->getWorldTransform();
	btTransform to = from;
	to.setOrigin(to.getOrigin() + moveVector);

	//sweepTest
	MyConvexClosestCallBack convex_cb(obj);
	this->bulletWorld->convexSweepTest(shape, from, to, convex_cb, allowDistance);


	if (convex_cb.hasHit()) {
		btVector3 objPos;
		objPos.setZero();
		obj->setWorldTransform(to);
		objPos.setInterpolate3(from.getOrigin(), to.getOrigin(), convex_cb.m_closestHitFraction);
		to.setOrigin(objPos);
		resultHitFraction = convex_cb.m_closestHitFraction;
	}
	obj->setWorldTransform(to);

	//衝突が解消されるまで法線方向に押し出す
	btVector3 prevPos = obj->getWorldTransform().getOrigin();
	MyContactCallBack contact_cb(obj);
	do {
		contact_cb.isHit = false;
		this->bulletWorld->contactTest(obj, contact_cb);
	} while (contact_cb.isHit);

	//小数点第四位以下を切り捨て
	btVector3 normal = obj->getWorldTransform().getOrigin() - prevPos;
	normal.setX((int)(normal.x() * 1000.0f) / 1000.0f);
	normal.setY((int)(normal.y() * 1000.0f) / 1000.0f);
	normal.setZ((int)(normal.z() * 1000.0f) / 1000.0f);

	if (normal.norm() < 0.001f) {
		return btVector3(0, 0, 0);
	}
	else {
		normal.normalize();
	}

	return normal;
#if 0
	//めり込み解消時のズレを補正（押し出した方向を「法線」から「移動方向の逆」に）
	btVector3 nowPosition     = obj->getWorldTransform().getOrigin();//現在位置
	btVector3 sweepMoveVector = prevPos - from.getOrigin();//sweepTestでの移動ベクトル
	btVector3 hPos            = prevPos + normal * -sweepMoveVector.dot(normal);//衝突平面と平行な、fromの位置を含む同一平面上に下ろした線の足

	btVector3 bh          = nowPosition - prevPos;
	btVector3 bhDash      = hPos - prevPos;
	btVector3 hDashp      = from.getOrigin() - hPos;

	printVec3(normal);
	if (hDashp.norm() < 0.001f) {
		return normal;
	}
	if (bhDash.norm() < 0.001f) {
		return normal;
	}
	btVector3 ha          = hDashp.normalized() * (bh.norm() * hDashp.norm()) / bhDash.norm();
	btVector3 fixPosition = nowPosition + ha;


	to.setOrigin(fixPosition);
	obj->setWorldTransform(to);

	return normal;
#endif
}

