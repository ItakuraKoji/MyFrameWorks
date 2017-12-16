#include"BulletPhysics.h"

BulletPhysics::BulletPhysics() {
	Initialize();
}
BulletPhysics::~BulletPhysics() {
	Finalize();
}

//bulletWorld�̏�����
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
		//�������Ȃ�
	}
	debugDrawer.SetShader(this->shader);
	return true;
}
//�J��
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

//�V�~�����[�V������i�߂�
void BulletPhysics::Run() {
	this->bulletWorld->stepSimulation(1 / 60.0f, 10, 1 / 60.0f);
}
//�f�o�b�O�`��
void BulletPhysics::DebugDraw(Matrix4f& world, Matrix4f& view, Matrix4f& projection) {
	//�V�F�[�_�[�̓ǂݍ��݂Ɏ��s�����Ƃ��̓f�o�b�O�`��ł��Ȃ�
	if (!this->shader) {
		return;
	}

	this->shader->UseShader();

	Matrix4f mat = projection * view * world;
	this->shader->SetMatrix(mat);
	this->bulletWorld->debugDrawWorld();
}

//�I�u�W�F�N�g����
//�O�p�`
btCollisionShape* BulletPhysics::CreateTriangleShape(btVector3& point1, btVector3& point2, btVector3& point3) {
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

//�J��
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

//�V�^�Փ˃V�~�����[�V����
void BulletPhysics::MoveCharacterObject(btCollisionObject *obj, btVector3& hMove, btVector3& vMove) {
	//���`��Ƃ͔���ł��Ȃ�
	if (!obj->getCollisionShape()->isConvex()) {
		return;
	}

	//�ړ��͏c�Ɖ��ŕ���
	btVector3 normal;
	btTransform prevTrans = obj->getWorldTransform();
	//�܂��@�����擾���Ă݂�
	normal = MoveConvexObject(obj, vMove, 40.0f);

	if (normal.norm() >= 0.001f) {
		//����̕�����
		if (normal.dot(vMove) > 0) {
			vMove = vMove.norm() * normal;
		}
		else if (normal.dot(vMove) < 0) {
			vMove = -vMove.norm() * normal;
		}

	}
	obj->setWorldTransform(prevTrans);
	//�c
	btVector3 virtical = vMove;
	MoveConvexObject(obj, virtical, 40.0f);

	//��
	//btVector3 horizontal = (moveVector.x()) * xAxis + (moveVector.z()) * zAxis;
	MoveConvexObject(obj, hMove, 0.0f);

}

void BulletPhysics::DiscreteMoveObject(btCollisionObject *obj, btVector3& hMove, btVector3& vMove) {
	//���`��Ƃ͔���ł��Ȃ�
	if (!obj->getCollisionShape()->isConvex()) {
		return;
	}
	//�ړ�
	btTransform to = obj->getWorldTransform();
	to.setOrigin(to.getOrigin() + hMove + vMove);
	obj->setWorldTransform(to);

	//�Փ˂����������܂Ŗ@�������ɉ����o��
	MyContactCallBack contact_cb(obj);
	do {
		contact_cb.isHit = false;
		this->bulletWorld->contactTest(obj, contact_cb);
	} while (contact_cb.isHit);

}


////////
//private
////

//�w������Ɉړ��i�ǂ���t���j
btVector3 BulletPhysics::MoveConvexObject(btCollisionObject *obj, btVector3 &moveVector, float limitAngle) {
	if (moveVector.norm() < 0.001f) {
		return btVector3(0, 0, 0);
	}

	float hitFraction = 0.0f;
	btVector3 normal;


	//�ړ�����
	normal = MoveSimulation(obj, moveVector, hitFraction);


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
	MoveSimulation(obj, goVec, hitFraction);


	//�ǂ��莞�͏Փ˖@����Ԃ��Ȃ�
	return btVector3(0, 0, 0);
}

//�ړ��������܂Ƃ�
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

	//�Փ˂����������܂Ŗ@�������ɉ����o��
	btVector3 prevPos = obj->getWorldTransform().getOrigin();
	MyContactCallBack contact_cb(obj);
	do {
		contact_cb.isHit = false;
		this->bulletWorld->contactTest(obj, contact_cb);
	} while (contact_cb.isHit);

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
#if 0
	//�߂荞�݉������̃Y����␳�i�����o�����������u�@���v����u�ړ������̋t�v�Ɂj
	btVector3 nowPosition     = obj->getWorldTransform().getOrigin();//���݈ʒu
	btVector3 sweepMoveVector = prevPos - from.getOrigin();//sweepTest�ł̈ړ��x�N�g��
	btVector3 hPos            = prevPos + normal * -sweepMoveVector.dot(normal);//�Փ˕��ʂƕ��s�ȁAfrom�̈ʒu���܂ޓ��ꕽ�ʏ�ɉ��낵�����̑�

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

