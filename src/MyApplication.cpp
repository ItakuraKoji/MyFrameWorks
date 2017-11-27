#include"MyApplication.h"

Vector3f pos(0.0f, 10.6f, 0.0f);
float rotation = 0.0f;
float v = 0.0f, g = 0.03f;
float camerarotH, camerarotV;



MyApplication::MyApplication()
{
}

MyApplication::~MyApplication()
{
	if (this->model) {
		delete this->model;
		this->model = NULL;
	}
	if (this->skinModel) {
		delete this->skinModel;
		this->skinModel = NULL;
	}
	if (this->mapModel) {
		delete this->mapModel;
		this->mapModel = NULL;
	}
	if (this->shader) {
		delete this->shader;
		this->shader = NULL;
	}
	if (this->camera) {
		delete this->camera;
		this->camera = NULL;
	}
	if (this->input) {
		delete this->input;
		this->input = NULL;
	}
	if (this->map) {
		delete this->map;
		this->map = NULL;
	}
	if (this->physics) {
		delete this->physics;
		this->physics = NULL;
	}
	if (this->light) {
		delete this->light;
		this->light = NULL;
	}
	delete this->buffer;
}

bool MyApplication::Initialize(int width, int height) {
	this->model = new Emitter;
	if (!this->model->Initialize()) {
		return false;
	}
	this->skinModel = new SkinModel;
	if (!this->skinModel->Initialize("KaminariChan.fbx")) {
		return false;
	}
	this->mapModel = new StaticModel;
	if (!this->mapModel->Initialize("Map.fbx")) {
		return false;
	}
	this->map = new MapPolygon;
	this->map->LoadModel("Map.fbx");

	this->shader = new ShaderList;
	this->shader->Initialize();
	if (!this->shader->Add("static", "Shader/StaticShader.vs", "Shader/StaticShader.ps")) {
		return false;
	}
	if (!this->shader->Add("skin", "Shader/SkinShader.vs", "Shader/SkinShader.ps")) {
		return false;
	}
	if (!this->shader->Add("simple", "Shader/SimpleShader.vs", "Shader/SimpleShader.ps")) {
		return false;
	}

	this->camera = new CameraClass;
	this->camera->SetPosition(0, -20, -1);

	this->input = new InputClass;
	this->input->Initialize(0);

	this->light = new DirectionalLight;
	this->light->SetDirection(0.0f, 0.0f, 1.0f);
	this->light->SetColor(1.0f, 1.0f, 1.0f, 1.0f);


	this->screenWidth = width;
	this->screenHeight = height;
	this->fieldOfView = (float)Noraml_Rad(45.0f);
	this->projectionMat = MatrixPerspectiveLH((float)this->screenWidth, (float)this->screenHeight, this->fieldOfView, 0.1f, 1000.0f);

	bulletInitialize();


	this->buffer = new Framebuffer;
	return true;
}

//とりあえず物理エンジンのコード（初期化）
void MyApplication::bulletInitialize() {
	//エンジンを初期化
	this->physics = new BulletPhysics;
	this->physics->Initialize();

	//コリジョンを用意
	{
		btCollisionShape *characterShape = new btCapsuleShape(btScalar(0.3f), btScalar(0.8f));
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(btVector3(pos.x(), pos.y(), pos.z()));

		btScalar mass(0.0f);
		btDefaultMotionState *state = new btDefaultMotionState(trans);
		btRigidBody::btRigidBodyConstructionInfo info(mass, state, characterShape, btVector3(0.0f, 0.0f, 0.0f));
		btRigidBody *rigid = new btRigidBody(info);
		this->physics->AddRigidBody(rigid, BulletBitMask::BIT_MAP, BulletBitMask::BIT_CHARACTER);
		this->characterCollision = rigid;
		rigid->setAngularFactor(0);
	}
	for (int i = 0; i < 1; ++i)
	{
		btCollisionShape* shape = new btSphereShape(btScalar(3.0f));
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(btVector3(4.0f * i, 20.0f, 0.0f));

		btScalar mass(1.0f);
		btVector3 inertia;
		shape->calculateLocalInertia(mass, inertia);
		btDefaultMotionState *state = new btDefaultMotionState(trans);
		btRigidBody::btRigidBodyConstructionInfo info(mass, state, shape, inertia);
		btRigidBody *rigid = new btRigidBody(info);
		this->physics->AddRigidBody(rigid, BulletBitMask::BIT_MAP, BulletBitMask::BIT_CHARACTER);

	}


	this->map->setCollisionWorld(this->physics);
	
}


void MyApplication::Run() {

	this->input->Run();
	//移動ベクトルと移動後座標ベクトル

	Vector3f fixedPosition;

	float srRotation = this->input->GetStickRotation(VPAD_STICK_R);
	float srPower = this->input->GetStickPower(VPAD_STICK_R);

	camerarotH += 2.0f * srPower * cosf(srRotation);
	camerarotV -= 2.0f * srPower * sinf(srRotation);

	float slRotation = this->input->GetStickRotation(VPAD_STICK_L);
	float slPower    = this->input->GetStickPower(VPAD_STICK_L);
	if (slPower) {
		rotation = slRotation + Noraml_Rad(90.0f) + Noraml_Rad(camerarotH);
	}

	Vector3f direction(0.0f, 0.0f, 0.0f);
	direction.x() += 0.4f * slPower * cosf(slRotation);
	direction.z() -= 0.4f * slPower * sinf(slRotation);

	if (this->input->isPressButton(VPAD_BUTTON_A)) {
		v = 1.0f;
	}
	v -= g;
	if (v < -1.0f) {
		v = -1.0f;
	}
	direction.y() += v;


	//移動ベクトルをカメラの軸に合わせる
	Vector3f xAxis = this->camera->GetAxisX();
	Vector3f zAxis = this->camera->GetAxisZ();
	xAxis.y() = 0.0f;
	zAxis.y() = 0.0f;
	Vector3f goVec = direction.x() * xAxis.normalized() + Vector3f(0, direction.y(), 0) + direction.z() * zAxis.normalized();

	////キャラクタの横移動と縦移動は分離
	//btVector3 angle = this->physics->MoveCharacterObject(this->characterCollision, btVector3(0.0f, goVec.y(), 0.0f), 40.0f);
	//btVector3 angleX(1.0f, 0.0f, 0.0f);
	//btVector3 angleZ(0.0f, 0.0f, 1.0f);
	//if (!angle.isZero()) {
	//	//床の角度から横移動ベクトルを作成(着地判定時)
	//	angleX = angle.cross(angleZ);
	//	angleZ = -angle.cross(angleX);
	//	v = 0.0f;
	//}
	////生成した横移動ベクトルに従って移動
	//this->physics->MoveCharacterObject(this->characterCollision, goVec.x() * angleX + goVec.z() * angleZ, 0.0f);

	this->physics->MoveCharacterObject(this->characterCollision, btVector3(goVec.x(), goVec.y(), goVec.z()));
	this->physics->Run();

	btTransform trans = this->characterCollision->getWorldTransform();
	Vector3f prevPos = pos;
	pos.x() = trans.getOrigin().x();
	pos.y() = trans.getOrigin().y() - 0.75f;
	pos.z() = trans.getOrigin().z();
	btRigidBody::upcast(this->characterCollision)->activate(true);

	float cx = -12.0f * sinf(camerarotH * (float)M_PI / 180.0f) + pos.x();
	float cy = -12.0f * sinf(camerarotV * (float)M_PI / 180.0f) + pos.y();
	float cz = -12.0f * cosf(camerarotH * (float)M_PI / 180.0f) * cosf(camerarotV * (float)M_PI / 180.0f) + pos.z();
	this->camera->SetPosition(cx, cy + 2.0f, cz);
	this->camera->SetTarget(pos.x(), pos.y() + 2.0f, pos.z());

	std::vector<btCollisionObject*> testArray;
	this->physics->FindConfrictionObjects(testArray, this->characterCollision);

	//this->model->Run();
	this->skinModel->Run();
}

void MyApplication::Draw() {
	//this->buffer->Bind();

	Matrix4f view;
	this->camera->Draw();
	view = this->camera->GetViewMatrix();

	Matrix4f world;
	Translation<float, 3> trans;
	DiagonalMatrix<float, 3> scale;
	Quaternionf rot;
	Affine3f mat;
	ShaderClass* shader;

	world = Matrix4f::Identity();
	trans = Translation<float, 3>(0.0f, 0.0f, 0.0f);
	scale = DiagonalMatrix<float, 3>(-1.0f, 1.0f, 1.0f);
	rot = AngleAxisf(Noraml_Rad(0.0f), Vector3f::UnitX());
	mat = trans * rot * scale;
	shader = this->shader->UseShader("skin");
	this->light->SetShader(shader);
	shader->SetVertexShaderSubroutine("NotSkinning");
	this->mapModel->SetShader(shader);
	this->mapModel->SetMatrix(mat.matrix(), view, projectionMat);
	this->mapModel->Draw();

	for (int i = 0; i < 1; ++i) {
		world = Matrix4f::Identity();
		trans = Translation<float, 3>(pos.x(), pos.y() + i * 0.1f, pos.z());
		scale = DiagonalMatrix<float, 3>(-1.0f, 1.0f, 1.0f);
		rot = AngleAxisf(Noraml_Rad(-90.0f), Vector3f::UnitX());
		rot = AngleAxisf(rotation, Vector3f::UnitY()) * rot;
		mat = trans * rot * scale;
		shader = this->shader->UseShader("skin");
		this->light->SetShader(shader);
		shader->SetVertexShaderSubroutine("CalcBoneMat");
		this->skinModel->SetShader(shader);
		this->skinModel->SetMatrix(mat.matrix(), view, projectionMat);
		//this->skinModel->Draw();
	}


	Matrix3f cameraInv = this->camera->GetViewMatrixInverse().block(0, 0, 3, 3);
	world = Matrix4f::Identity();
	trans = Translation<float, 3>(0, 0.0f, 0.0f);
	scale = DiagonalMatrix<float, 3>(1.0f, 1.0f, 1.0f);
	rot = AngleAxisf(Noraml_Rad(0.0f), Vector3f::UnitX());
	mat = trans * cameraInv * rot * scale;
	shader = this->shader->UseShader("static");
	this->model->SetShader(shader);
	this->model->SetMatrix(mat.matrix(), view, projectionMat);
	this->model->Draw();

	//デバッグ用コリジョン描画
	world = Matrix4f::Identity();
	trans = Translation<float, 3>(0, 0, 0);
	scale = DiagonalMatrix<float, 3>(1.0f, 1.0f, 1.0f);
	rot = AngleAxisf(Noraml_Rad(0.0f), Vector3f::UnitX());
	mat = trans * rot * scale;
	//this->physics->DebugDraw(mat.matrix(), view, projectionMat);
	
	//this->buffer->UnBind();
}

Matrix4f MyApplication::MatrixPerspectiveLH(float screenWidth, float screenHeight, float fieldOfView, float screenNear, float screenFar) {
	Matrix4f mat;
	mat(0) = screenHeight / (screenWidth * tanf(fieldOfView * 0.5f));
	mat(1) = 0.0f;
	mat(2) = 0.0f;
	mat(3) = 0.0f;

	mat(4) = 0.0f;
	mat(5) = 1.0f / tanf(fieldOfView * 0.5f);
	mat(6) = 0.0f;
	mat(7) = 0.0f;

	mat(8) = 0.0f;
	mat(9) = 0.0f;
	mat(10) = screenFar / (screenFar - screenNear);
	mat(11) = 1.0f;

	mat(12) = 0.0f;
	mat(13) = 0.0f;
	mat(14) = screenFar * -screenNear / (screenFar - screenNear);
	mat(15) = 0.0f;
	return mat;
}