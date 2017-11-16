#include"MyApplication.h"

float x = 0.0f, y = 10.6f, z = 0.0f;
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
	if (!this->shader->Add("static", "StaticShader.vs", "StaticShader.ps")) {
		return false;
	}
	if (!this->shader->Add("skin", "SkinShader.vs", "SkinShader.ps")) {
		return false;
	}
	if (!this->shader->Add("simple", "SimpleShader.vs", "SimpleShader.ps")) {
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
		trans.setOrigin(btVector3(x, y, z));

		btCollisionObject *obj = new btCollisionObject;
		obj->setCollisionShape(characterShape);
		obj->setWorldTransform(trans);


		this->physics->AddCollisionObject(obj);
		this->characterCollision = obj;
	}


	this->map->setCollisionWorld(this->physics);
	
}


void MyApplication::Run() {

	this->input->Run();
	//移動ベクトルと移動後座標ベクトル

	Vector3f direction(0.0f, 0.0f, 0.0f);
	Vector3f fixedPosition;

	float au = this->input->GetStickState(VPAD_STICK_RX);
	float av = this->input->GetStickState(VPAD_STICK_RY);

	camerarotH += au * 2.0f;
	camerarotV -= av * 2.0f;

	direction.x() += 0.5f * this->input->GetStickState(VPAD_STICK_LX);
	direction.z() += 0.5f * this->input->GetStickState(VPAD_STICK_LY);
	if (this->input->isPressButton(VPAD_BUTTON_A)) {
		v = 1.0f;
	}
	v -= g;
	direction.y() += v;

	Vector3f xAxis = this->camera->GetAxisX();
	Vector3f zAxis = this->camera->GetAxisZ();

	xAxis.y() = 0.0f;
	zAxis.y() = 0.0f;
	Vector3f goVec = direction.x() * xAxis.normalized() + Vector3f(0, direction.y(), 0) + direction.z() * zAxis.normalized();

	this->physics->Run();
	if (this->physics->MoveCharacterObject(this->characterCollision, btVector3(0.0f, goVec.y(), 0.0f))) {
		v = 0.0f;
	}

	this->physics->MoveCharacterObject(this->characterCollision, btVector3(goVec.x(), 0.0f, goVec.z()));

	btTransform trans = this->characterCollision->getWorldTransform();
	x = trans.getOrigin().x();
	y = trans.getOrigin().y() - 0.75f;
	z = trans.getOrigin().z();

	float cx = -12.0f * sinf(camerarotH * (float)M_PI / 180.0f) + x;
	float cy = -12.0f * sinf(camerarotV * (float)M_PI / 180.0f) + y;
	float cz = -12.0f * cosf(camerarotH * (float)M_PI / 180.0f) * cosf(camerarotV * (float)M_PI / 180.0f) + z;
	this->camera->SetPosition(cx, cy + 2.0f, cz);
	this->camera->SetTarget(x, y + 2.0f, z);

	this->model->Run();
	this->skinModel->Run();
}

void MyApplication::Draw() {


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
	shader = this->shader->GetShader("skin");
	shader->SetShader();
	this->light->SetShader(shader);
	shader->SetSkinningFunc(1);
	this->mapModel->SetShader(shader);
	this->mapModel->SetMatrix(mat.matrix(), view, projectionMat);
	this->mapModel->Draw();

	world = Matrix4f::Identity();
	trans = Translation<float, 3>(x, y, z);
	scale = DiagonalMatrix<float, 3>(-1.0f, 1.0f, 1.0f);
	rot = AngleAxisf(Noraml_Rad(-90.0f), Vector3f::UnitX());
	mat = trans * rot * scale;
	shader = this->shader->GetShader("skin");
	shader->SetShader();
	this->light->SetShader(shader);
	shader->SetSkinningFunc(0);
	this->skinModel->SetShader(shader);
	this->skinModel->SetMatrix(mat.matrix(), view, projectionMat);
	this->skinModel->Draw();


	Matrix3f cameraInv = this->camera->GetViewMatrixInverse().block(0, 0, 3, 3);
	world = Matrix4f::Identity();
	trans = Translation<float, 3>(0, 0.0f, 0.0f);
	scale = DiagonalMatrix<float, 3>(1.0f, 1.0f, 1.0f);
	rot = AngleAxisf(Noraml_Rad(0.0f), Vector3f::UnitX());
	mat = trans * cameraInv * rot * scale;
	shader = this->shader->GetShader("static");
	this->model->SetShader(shader);
	this->model->SetMatrix(mat.matrix(), view, projectionMat);
	this->model->Draw();


	world = Matrix4f::Identity();
	trans = Translation<float, 3>(0, 0, 0);
	scale = DiagonalMatrix<float, 3>(1.0f, 1.0f, 1.0f);
	rot = AngleAxisf(Noraml_Rad(0.0f), Vector3f::UnitX());
	mat = trans * rot * scale;
	shader = this->shader->GetShader("simple");
	shader->SetShader();
	shader->SetShaderParameter(mat.matrix().data(), view.data(), projectionMat.data());
	this->physics->DebugDraw(mat.matrix().data(), view.data(), projectionMat.data());



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