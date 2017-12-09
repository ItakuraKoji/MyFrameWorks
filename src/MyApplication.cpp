#include"MyApplication.h"

Vector3f pos(0.0f, 10.6f, 0.0f);
float rotation = 0.0f;
float v = 0.0f, g = 0.03f;
float camerarotH, camerarotV;



MyApplication::MyApplication() {
}

MyApplication::~MyApplication() {
	Finalize();
}

bool MyApplication::Initialize(int width, int height) {
	try {


		this->texture = new TextureList;
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
		if (!this->shader->Add("screen", "Shader/ScreenShader.vs", "Shader/ScreenShader.ps")) {
			return false;
		}

		this->param.shaderList = this->shader;
		this->param.textureList = this->texture;
		this->param.screenWidth = width;
		this->param.screenHeight = height;

		ModelDataFactory factory;
		this->skinModel = new MeshModel(factory.LoadFBXModel("KaminariChan.fbx", this->param));
		this->skinModel->SetAnimation("Dash", true, false, true);
		this->mapModel = new MeshModel(factory.LoadFBXModel("Map.fbx", this->param));

		this->model = new Emitter;
		if (!this->model->Initialize(this->param)) {
			return false;
		}


		this->map = new MapPolygon;
		this->map->LoadModel("Map.fbx");



		this->camera = new CameraClass;
		this->camera->SetPosition(0, -20, -1);

		this->input = new InputClass(0);

		this->light = new DirectionalLight;
		this->light->SetDirection(0.0f, 0.0f, 1.0f);
		this->light->SetColor(1.0f, 1.0f, 1.0f, 1.0f);


		this->screenWidth = width;
		this->screenHeight = height;
		this->fieldOfView = (float)Noraml_Rad(45.0f);
		this->projectionMat = MatrixPerspectiveLH((float)this->screenWidth, (float)this->screenHeight, this->fieldOfView, 0.1f, 1000.0f);

		bulletInitialize();

		this->buffer = new Framebuffer(this->texture, "frameBuffer", 1000, 1000);

		this->square = new MeshModel(factory.CreateSquareModel("frameBuffer", this->param));


	}
	catch (char* eText) {
		return false;
	}
	return true;
}

void MyApplication::Finalize() {
	if (this->texture) {
		delete this->texture;
		this->texture = NULL;
	}
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
	if (this->buffer) {
		delete this->buffer;
	}
	if (this->square) {
		delete this->square;
	}
}



void MyApplication::Run() {

	this->input->Run();
	//�ړ��x�N�g���ƈړ�����W�x�N�g��

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


	//�ړ��x�N�g�����J�����̎��ɍ��킹��
	Vector3f xAxis = this->camera->GetAxisX();
	Vector3f zAxis = this->camera->GetAxisZ();
	xAxis.y() = 0.0f;
	zAxis.y() = 0.0f;
	Vector3f goVec = direction.x() * xAxis.normalized() + Vector3f(0, direction.y(), 0) + direction.z() * zAxis.normalized();


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

	this->model->Run();
}

void MyApplication::Draw() {
	this->buffer->Bind();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glViewport(0, 0, this->buffer->GetWidth(), this->buffer->GetHeight());
	DrawPass0();
	this->buffer->UnBind();
	glClearColor(0.3f, 0.5f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glViewport(0, 0, 1000, 1000);
	DrawPass1();
}

////////
//private
////

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

//�Ƃ肠���������G���W���̃R�[�h�i�������j
void MyApplication::bulletInitialize() {
	//�G���W����������
	this->physics = new BulletPhysics;

	//�R���W������p��
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

//�`��p�X
void MyApplication::DrawPass0() {
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
	shader->SetMatrix(projectionMat * view * mat.matrix());
	shader->SetWorldMatrix(mat.matrix());
	this->mapModel->Draw(param, "skin");

	for (int i = 0; i < 1; ++i) {
		world = Matrix4f::Identity();
		trans = Translation<float, 3>(pos.x(), pos.y() + i * 0.1f, pos.z());
		scale = DiagonalMatrix<float, 3>(-1.0f, 1.0f, 1.0f);

		rot = AngleAxisf(Noraml_Rad(-90.0f), Vector3f::UnitX());
		rot = AngleAxisf(rotation, Vector3f(0.0f, 1.0f, 0.0f)) * rot;
		mat = trans * rot * scale;
		shader = this->shader->UseShader("skin");
		this->light->SetShader(shader);
		shader->SetVertexShaderSubroutine("CalcBoneMat");
		shader->SetMatrix(projectionMat * view * mat.matrix());
		shader->SetWorldMatrix(mat.matrix());
		this->skinModel->Draw(param, "skin");
	}


	Matrix3f cameraInv = this->camera->GetViewMatrixInverse().block(0, 0, 3, 3);
	world = Matrix4f::Identity();
	trans = Translation<float, 3>(0, 0.0f, 0.0f);
	scale = DiagonalMatrix<float, 3>(1.0f, 1.0f, 1.0f);
	rot = AngleAxisf(Noraml_Rad(0.0f), Vector3f::UnitX());
	mat = trans * rot * scale;
	shader = this->shader->UseShader("static");
	this->model->SetMatrix(mat.matrix(), view, projectionMat);

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	this->model->Draw(this->param);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	//�f�o�b�O�p�R���W�����`��
	world = Matrix4f::Identity();
	trans = Translation<float, 3>(0, 0, 0);
	scale = DiagonalMatrix<float, 3>(1.0f, 1.0f, 1.0f);
	rot = AngleAxisf(Noraml_Rad(0.0f), Vector3f::UnitX());
	mat = trans * rot * scale;
	//this->physics->DebugDraw(mat.matrix(), view, projectionMat);
}
void MyApplication::DrawPass1() {
	Matrix4f view = Matrix4f::Identity();
	view(14) = 0.5f;

	Affine3f world;
	Translation<float, 3> trans = Translation<float, 3>(0, 0, 0);
	DiagonalMatrix<float, 3> scale = DiagonalMatrix<float, 3>(2, 2, 2);
	world = trans * scale;

	ShaderClass* shader = this->shader->UseShader("screen");
	shader->SetMatrix(projectionMat * view * world.matrix());
	this->square->Draw(this->param, "screen");

}
void MyApplication::DrawPass2() {

}
