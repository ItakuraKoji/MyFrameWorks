#include"BulletPhysics.h"

BulletPhysics::BulletPhysics() {

}
BulletPhysics::~BulletPhysics() {
	for (int i = this->bulletWorld->getNumCollisionObjects() -1; i >= 0; --i) {
		btCollisionObject *obj = this->bulletWorld->getCollisionObjectArray()[i];
		btRigidBody       *rigid = btRigidBody::upcast(obj);
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

//bulletWorld�̏�����
bool BulletPhysics::Initialize() {
	this->config      = new btDefaultCollisionConfiguration;
	this->dispatcher  = new btCollisionDispatcher(this->config);
	this->broadphase  = new btDbvtBroadphase;
	this->solver      = new btSequentialImpulseConstraintSolver;
	this->bulletWorld = new btDiscreteDynamicsWorld(this->dispatcher, this->broadphase, this->solver, this->config);
	
	this->debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	this->bulletWorld->setDebugDrawer(&this->debugDrawer);

	this->bulletWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));

	this->shader = new ShaderClass;
	if (!this->shader->Initialize("SimpleShader.vs", "SimpleShader.ps")) {
		return false;
	}
	return true;
}
//�V�~�����[�V������i�߂�
void BulletPhysics::Run() {
	this->bulletWorld->stepSimulation(1 / 60.0f, 10, 1 / 60.0f);
}
//�f�o�b�O�`��
void BulletPhysics::DebugDraw(float *world, float *view, float *projection) {
	this->shader->SetShader();
	this->shader->SetShaderParameter(world, view, projection);
	this->bulletWorld->debugDrawWorld();
}

void BulletPhysics::AddRigidBody(btRigidBody *rigidbody) {
	this->shapeArray.push_back(rigidbody->getCollisionShape());
	this->bulletWorld->addRigidBody(rigidbody);
}
void BulletPhysics::AddCollisionObject(btCollisionObject *obj) {
	this->shapeArray.push_back(obj->getCollisionShape());
	this->bulletWorld->addCollisionObject(obj);
}
void BulletPhysics::RemoveRigidBody(btRigidBody *rigidbody) {

}
void BulletPhysics::RemoveCollisionObject(btCollisionObject *obj) {

}

//�ʌ`��̃R���W�����𓮂����B���̍ۂɁA�Փ˂�����������true��Ԃ�
bool BulletPhysics::MoveCharacterObject(btCollisionObject *obj, btVector3 &moveVector) {
	btCollisionShape *shape = obj->getCollisionShape();
	if (!shape->isConvex()) {
		return false;
	}
	btTransform from = obj->getWorldTransform();
	btTransform to = from;
	to.setOrigin(to.getOrigin() + moveVector);
	if (from == to || moveVector.isZero()) {
		return false;
	}

	MyConvexClosestCallBack cb(obj);


	this->bulletWorld->convexSweepTest((btConvexShape*)shape, from, to, cb);
	
	//���R�Ȃ��玩�����g�Ƃ͔�������Ȃ�
	if (cb.hasHit()) {
		btVector3 objPos;
		objPos.setInterpolate3(from.getOrigin(), to.getOrigin(), (cb.m_closestHitFraction));

		//�c��̃x�N�g���i�ǂ���x�N�g���p�j
		btVector3 vec = to.getOrigin() - objPos;
		btScalar a = -vec.dot(cb.m_hitNormalWorld)+0.01f;
		objPos += vec + cb.m_hitNormalWorld * a;

		from.setOrigin(objPos);
		obj->setWorldTransform(from);
		return true;
	}
	
	obj->setWorldTransform(to);
	return false;
}