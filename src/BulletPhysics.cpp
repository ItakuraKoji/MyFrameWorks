#include"BulletPhysics.h"

////////
//public
////

BulletPhysics::BulletPhysics() {
	Initialize();
}
BulletPhysics::~BulletPhysics() {
	Finalize();
}

//bulletWorld�̏�����
bool BulletPhysics::Initialize() {
	try {
		Finalize();

		this->bulletWorld = nullptr;
		this->solver = nullptr;
		this->broadphase = nullptr;
		this->dispatcher = nullptr;
		this->config = nullptr;

		this->config = new btDefaultCollisionConfiguration;
		this->dispatcher = new btCollisionDispatcher(this->config);
		this->broadphase = new btDbvtBroadphase;
		this->solver = new btSequentialImpulseConstraintSolver;
		this->bulletWorld = new btDiscreteDynamicsWorld(this->dispatcher, this->broadphase, this->solver, this->config);

		this->debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawContactPoints);
		this->bulletWorld->setDebugDrawer(&this->debugDrawer);

		this->bulletWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));

	}
	catch(...){
		//�������Ȃ�
	}
	return true;
}
//�J��
void BulletPhysics::Finalize() {

	for (int i = 0; i < this->shapeArray.size(); ++i) {
		delete this->shapeArray[i];
	}

	if (this->bulletWorld != nullptr) {
		for (int i = this->bulletWorld->getNumCollisionObjects() - 1; i >= 0; --i) {
			btCollisionObject* obj = this->bulletWorld->getCollisionObjectArray()[i];
			RemoveCollisionObject(obj);
		}
		delete this->bulletWorld;
	}
	if (this->solver != nullptr) {
		delete this->solver;
	}
	if (this->broadphase != nullptr) {
		delete this->broadphase;
	}
	if (this->dispatcher != nullptr) {
		delete this->dispatcher;
	}
	if (this->config != nullptr) {
		delete this->config;
	}
}

//�V�~�����[�V������i�߂�
void BulletPhysics::Run() {
	this->bulletWorld->stepSimulation(1 / 60.0f, 10, 1 / 60.0f);
}
//�f�o�b�O�`��
void BulletPhysics::DebugDraw(ShaderClass* shader, CameraClass* camera, Matrix4f& trans) {
	glDisable(GL_DEPTH_TEST);

	shader->UseShader();
	shader->SetVertexShaderSubroutine("NotSkinning");
	shader->SetFragmentShaderSubroutine("None");

	Matrix4f projection;
	M::MatrixPerspectiveLH(projection, camera->GetScreenWidth(), camera->GetScreenHeight(), 0.1f, 200.0f, camera->GetFieldOfView());

	Matrix4f mat = projection * camera->GetViewMatrix() * trans;
	shader->SetMatrix(mat);
	this->bulletWorld->debugDrawWorld();
	glEnable(GL_DEPTH_TEST);
}

//�I�u�W�F�N�g����
btTriangleMesh* BulletPhysics::CreateTriangleMesh(btVector3* vectice, int numFace) {
	btTriangleMesh *mesh = new btTriangleMesh;

	for (int i = 0; i < numFace; ++i) {
		mesh->addTriangle(vectice[i * 3], vectice[i * 3 + 1], vectice[i * 3 + 2], false);
	}
	return mesh;
}
//�����̎O�p�`����Ȃ郁�b�V���R���W����
btCollisionShape* BulletPhysics::CreateTriangleMeshShape(btTriangleMesh* mesh) {
	btBvhTriangleMeshShape* bvhMesh = new btBvhTriangleMeshShape(mesh, true);
	this->shapeArray.push_back(bvhMesh);
	//btTriangleInfoMap* info = new btTriangleInfoMap;
	//btGenerateInternalEdgeInfo(bvhMesh, info);
	//this->shapeArray.push_back(bvhMesh);
	return bvhMesh;
}
//�O�p�`
btCollisionShape* BulletPhysics::CreateTriangleHullShape(btVector3& point1, btVector3& point2, btVector3& point3) {
	btScalar points[9] = { point1.x(),point1.y(),point1.z(),
		                   point2.x(),point2.y(),point2.z(),
		                   point3.x(),point3.y(),point3.z() };
	btCollisionShape* shape = new btConvexHullShape(points, 3, 3 * sizeof(btScalar));
	this->shapeArray.push_back(shape);
	return shape;
}
//��
btCollisionShape* BulletPhysics::CreateSphereShape(float radius) {
	btCollisionShape* shape = new btSphereShape(radius);
	this->shapeArray.push_back(shape);
	return shape;
}
//�J�v�Z��
btCollisionShape* BulletPhysics::CreateCapsuleShape(float radius, float height) {
	btCollisionShape* shape = new btCapsuleShape(radius, height);
	this->shapeArray.push_back(shape);
	return shape;
}
//�����`
btCollisionShape* BulletPhysics::CreateBoxShape(float halfWidth, float halfHeight, float halfDepth) {
	btCollisionShape* shape = new btBoxShape(btVector3(halfWidth, halfHeight, halfDepth));
	this->shapeArray.push_back(shape);
	return shape;
}

//����
CollisionData* BulletPhysics::CreateRigidBody(btCollisionShape* shape, btScalar mass, int mask, btVector3& pos, btVector3& rot) {
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

	this->bulletWorld->addRigidBody(rigid, 1, mask);

	CollisionTag tag = { "default", 0, nullptr };
	CollisionData* colData = new CollisionData(rigid, tag);
	rigid->setUserPointer(colData);
	return colData;

}

CollisionData* BulletPhysics::CreateCollisionObject(btCollisionShape* shape, bool ghost, int mask, btVector3& pos, btVector3& rot) {
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(pos);
	btQuaternion q = btQuaternion(btVector3(0, 0, 1), rot.z()) * btQuaternion(btVector3(1, 0, 0), rot.x()) * btQuaternion(btVector3(0, 1, 0), rot.y());
	trans.setRotation(q);

	btCollisionObject *collision = new btCollisionObject;
	collision->setCollisionShape(shape);
	if (ghost) {
		collision->setCollisionFlags(collision->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
	else {
		collision->setCollisionFlags(collision->getCollisionFlags() & !btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
	collision->setWorldTransform(trans);
	this->bulletWorld->addCollisionObject(collision, 0, 0);

	CollisionTag tag = {"default", 0, nullptr};
	CollisionData* colData = new CollisionData(collision, tag);
	collision->setUserPointer(colData);
	return colData;
}

//�J��
void BulletPhysics::RemoveCollisionObject(btCollisionObject* obj) {
	btRigidBody* rigid = btRigidBody::upcast(obj);
	if (rigid != nullptr) {
		if (rigid->getMotionState()) {
			delete rigid->getMotionState();
		}
	}
	if (obj->getUserPointer()) {
		delete (CollisionData*)obj->getUserPointer();
	}
	this->bulletWorld->removeCollisionObject(obj);
	delete obj;
}
void BulletPhysics::RemoveCollisionShape(btCollisionShape* shape) {
	this->shapeArray.remove(shape);
	delete shape;
	shape = 0;
}

//�Փ˂����o���A���ʂ��|�C���^�ŕԂ�
std::vector<CollisionTag>& BulletPhysics::FindConfrictionObjects(btCollisionObject* myself) {
	CollectCollisionCallBack callback(this->confrictResult);
	this->bulletWorld->contactTest(myself, callback);
	return this->confrictResult;
}

//�V�^�Փ˃V�~�����[�V����
void BulletPhysics::MoveCharacter(btCollisionObject *obj, btVector3& hMove, btVector3& vMove) {
	//���`��Ƃ͔���ł��Ȃ�
	if (!obj->getCollisionShape()->isConvex()) {
		return;
	}

	//�ړ��͏c�Ɖ��ŕ���
	btVector3 normal;
	btTransform prevTrans = obj->getWorldTransform();
	//�܂��@�����擾���Ă݂�
	normal = MoveSmooth(obj, vMove, 40.0f);

	if (normal.norm() >= 0.001f) {
		//����̕�����
		if (normal.dot(vMove) > 0) {
			vMove = vMove.norm() * normal;
			if (hMove.norm() > 0.01f) {
				btVector3 x = hMove.cross(normal);
				hMove = hMove.norm() * (x.cross(vMove)).normalized();
			}
		}
		else if (normal.dot(vMove) < 0) {
			vMove = -vMove.norm() * normal;
			if (hMove.norm() > 0.01f) {
				btVector3 x = hMove.cross(normal);
				hMove = hMove.norm() * (x.cross(vMove)).normalized();
			}
		}

	}
	obj->setWorldTransform(prevTrans);
	//�c
	btVector3 virtical = vMove;
	MoveSmooth(obj, virtical, 40.0f);

	//��
	btVector3 horizontal = hMove;
	MoveSmooth(obj, hMove, 0.0f);

}

//�������@�y���ق�
void BulletPhysics::MoveCharacterDiscrete(btCollisionObject *obj, btVector3& hMove, btVector3& vMove) {
	int fix = 10;

	//���`��Ƃ͔���ł��Ȃ�
	if (!obj->getCollisionShape()->isConvex()) {
		return;
	}
	btVector3 horizontal = hMove / (float)fix;
	//�ړ�
	for (int i = 0; i < fix; ++i) {
		btTransform to = obj->getWorldTransform();
		to.setOrigin(to.getOrigin() + horizontal);
		obj->setWorldTransform(to);
		//��Ԑ[���߂荞�񂾂��̖̂@�������։����o��
		FixContactCallBack contact_cb(obj);
		do {
			this->bulletWorld->contactTest(obj, contact_cb);
		} while (contact_cb.isLoop);
		//�����o��
		MoveCollisionObject(obj, contact_cb.fixVec * -contact_cb.maxDistance);
	}

	//�ړ�
	btVector3 vertical = vMove / (float)fix;
	for (int i = 0; i < fix; ++i) {
		btTransform to = obj->getWorldTransform();
		to.setOrigin(to.getOrigin() + vertical);
		obj->setWorldTransform(to);
		//��Ԑ[���߂荞�񂾂��̖̂@�������։����o��
		FixContactCallBack contact_cb(obj);
		do {
			this->bulletWorld->contactTest(obj, contact_cb);
		} while (contact_cb.isLoop);
		//�����o��
		MoveCollisionObject(obj, contact_cb.fixVec * -contact_cb.maxDistance);
	}
}


////////
//private
////

void BulletPhysics::MoveCollisionObject(btCollisionObject* obj, btVector3& moveVector) {
	btVector3 objPos = obj->getWorldTransform().getOrigin();
	objPos = objPos + moveVector;
	btTransform trans = obj->getWorldTransform();
	trans.setOrigin(objPos);
	obj->setWorldTransform(trans);
}


btVector3 BulletPhysics::MoveSmooth(btCollisionObject *obj, btVector3 &moveVector, float limitAngle) {
	if (moveVector.norm() < 0.001f) {
		return btVector3(0, 0, 0);
	}

	float hitFraction = 0.0f;
	btVector3 normal;


	//�ړ�����
	btVector3 prevPos = obj->getWorldTransform().getOrigin();
	normal = MoveBySweep(obj, moveVector);
	hitFraction = (obj->getWorldTransform().getOrigin() - prevPos).norm() / moveVector.norm();

	//�ǂ�������p�x�����m�F
	float angle_cos = (float)-normal.dot(moveVector.normalized());
	float limit_cos = (float)btCos(btRadians(limitAngle));
	if (abs(angle_cos - limit_cos) < 0.0001f) {
		angle_cos = limit_cos;
	}
	if (angle_cos > limit_cos || normal.norm() < 0.001f) {
		return normal;
	}

	//�ǂ�������
	btVector3 goVec;
	goVec.setZero();
	goVec = moveVector * (1.0f - hitFraction);
	goVec += normal * (-goVec.dot(normal));

	if (goVec.norm() < 0.001f) {
		return btVector3(0, 0, 0);
	}

	//�ړ�����
	MoveBySweep(obj, goVec);


	//�ǂ��莞�͏Փ˖@����Ԃ��Ȃ�
	return btVector3(0, 0, 0);
}

btVector3 BulletPhysics::MoveBySweep(btCollisionObject *obj, btVector3 &moveVector, float allowDistance){
	btConvexShape* shape = (btConvexShape*)obj->getCollisionShape();

	btTransform from = obj->getWorldTransform();
	btTransform to = from;
	to.setOrigin(to.getOrigin() + moveVector);

	//sweepTest
	SweepTestCallBack convex_cb(obj);
	this->bulletWorld->convexSweepTest(shape, from, to, convex_cb, allowDistance);

	//sweep���ʔ��f
	if (convex_cb.hasHit()) {
		btVector3 objPos;
		objPos.setZero();
		obj->setWorldTransform(to);
		objPos.setInterpolate3(from.getOrigin(), to.getOrigin(), convex_cb.m_closestHitFraction);
		to.setOrigin(objPos);
	}
	obj->setWorldTransform(to);

	//�w��񐔉����o��(���͏\��)
	//��Ԑ[���߂荞�񂾂��̖̂@�������։����o��
	btVector3 prevPos = obj->getWorldTransform().getOrigin();
	for (int i = 0; i < 10; ++i) {
		FixContactCallBack contact_cb(obj);
		do {
			this->bulletWorld->contactTest(obj, contact_cb);
		} while (contact_cb.isLoop);
		//�����o��
		MoveCollisionObject(obj, contact_cb.fixVec * -contact_cb.maxDistance);
	}

	//�����_��l�ʈȉ���؂�̂�
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
}

