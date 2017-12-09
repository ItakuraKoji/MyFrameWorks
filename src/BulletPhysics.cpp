#include"BulletPhysics.h"

void printVec3(btVector3& vec) {
	printf("%+.4f, %+.4f, %+.4f\n", vec.x(), vec.y(), vec.z());
}

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
		this->shader = new ShaderClass("SimpleShader.vs", "SimpleShader.ps");
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
		btCollisionObject* obj   = this->bulletWorld->getCollisionObjectArray()[i];
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

void BulletPhysics::AddRigidBody(btRigidBody *rigidbody, int confrictMask, int hitMask) {
	this->shapeArray.push_back(rigidbody->getCollisionShape());
	this->bulletWorld->addRigidBody(rigidbody, confrictMask, confrictMask | hitMask);
	rigidbody->setUserIndex(confrictMask);
	rigidbody->setUserIndex2(hitMask);
}
void BulletPhysics::AddCollisionObject(btCollisionObject *obj) {
	this->shapeArray.push_back(obj->getCollisionShape());
	this->bulletWorld->addCollisionObject(obj);
}
void BulletPhysics::RemoveRigidBody(btRigidBody *rigidbody) {

}
void BulletPhysics::RemoveCollisionObject(btCollisionObject *obj) {

}

bool BulletPhysics::FindConfrictionObjects(std::vector<btCollisionObject*>& resultArray, btCollisionObject* myself) {
	return true;
}

//新型衝突シミュレーション
void BulletPhysics::MoveCharacterObject(btCollisionObject *obj, btVector3 &moveVector, btVector3& xAxis, btVector3& yAxis, btVector3& zAxis) {
	if (!obj->getCollisionShape()->isConvex()) {
		return;
	}

	//移動は縦と横で分離
	btVector3 normal;
	btTransform prevTrans = obj->getWorldTransform();
	//まず法線を取得してみる
	normal = MoveConvexObject(obj, -yAxis * 0.1f, 40.0f);
	if (normal.norm() >= 0.001f) {
		//床基準の方向軸
		xAxis = normal.cross(zAxis);
		zAxis = -normal.cross(xAxis);
		yAxis = -xAxis.cross(zAxis);

	}
	obj->setWorldTransform(prevTrans);
	btVector3 virtical = moveVector.y() * yAxis;
	//縦
	MoveConvexObject(obj, virtical, 40.0f);
	//横
	btVector3 horizontal = (moveVector.x()) * xAxis + (moveVector.z()) * zAxis;
	MoveConvexObject(obj, horizontal, 0.0f);
}
//指定方向に移動（壁ずり付き）
btVector3 BulletPhysics::MoveConvexObject(btCollisionObject *obj, btVector3 &moveVector, float limitAngle) {
	if (moveVector.norm() < 0.001f) {
		return btVector3(0, 0, 0);
	}

	float hitFraction = 1.0f;
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
	const float allowDistance = 0.1f;
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

