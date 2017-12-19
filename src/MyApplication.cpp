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
		this->map = new MapPolygon;
		this->map->LoadModel("Map.fbx");
		bulletInitialize();
		this->param.gravity = 0.03f;
		this->param.screenWidth = width;
		this->param.screenHeight = height;
		this->param.screenFov = DegToRad(45.0f);
		this->param.input = new InputClass(0);
		this->param.textureList = new TextureList;
		this->param.shaderList = new ShaderList;
		this->param.shaderList->Initialize();

		this->param.shaderList->AddVertexShader("Shader/StaticShader.vs");
		this->param.shaderList->AddVertexShader("Shader/SkinShader.vs");
		this->param.shaderList->AddVertexShader("Shader/SimpleShader.vs");
		this->param.shaderList->AddVertexShader("Shader/ScreenShader.vs");

		this->param.shaderList->AddPixelShader("Shader/StaticShader.ps");
		this->param.shaderList->AddPixelShader("Shader/SkinShader.ps");
		this->param.shaderList->AddPixelShader("Shader/SimpleShader.ps");
		this->param.shaderList->AddPixelShader("Shader/ScreenShader.ps");


		if (!this->param.shaderList->Add("static", "Shader/StaticShader.vs", "Shader/StaticShader.ps")) {
			return false;
		}
		if (!this->param.shaderList->Add("skin", "Shader/SkinShader.vs", "Shader/SkinShader.ps")) {
			return false;
		}
		if (!this->param.shaderList->Add("simple", "Shader/SimpleShader.vs", "Shader/SimpleShader.ps")) {
			return false;
		}
		if (!this->param.shaderList->Add("screen", "Shader/ScreenShader.vs", "Shader/ScreenShader.ps")) {
			return false;
		}


		this->buffer = new Framebuffer(this->param.textureList, "frameBuffer", 1000, 1000);

		ModelDataFactory factory;
		this->mapModel = new MeshModel(factory.LoadFBXModel("Map.fbx", this->param));
		this->square = new MeshModel(factory.CreateSquareModel("frameBuffer", this->param));

		this->skinModel = new MeshModel(factory.LoadFBXModel("KaminariChan.fbx", this->param));

		this->camera = new CameraClass(ProjectionType::Perspective, this->param.screenWidth, this->param.screenHeight, 0.1f, 1000.0f, this->param.screenFov);
		this->camera->SetPosition(0, -20, -1);
		this->camera->Draw();

		this->player = new Player();
		this->player->SetPosition(0.0f, 10.6f, 0.0f);
		this->player->SetDrawModel(this->skinModel);
		this->player->SetShaderName("skin");
		this->player->Initialize(this->param);

		this->player->SetCameraMan(this->camera);

		this->model = new Emitter;
		if (!this->model->Initialize(this->param)) {
			return false;
		}

		this->param.lightList = new LightList;
		this->param.lightList->AddAmbient("ambient", 0.6f, Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
		this->param.lightList->AddDirectional("directional", 1.0f, Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Vector3f(0.0f, 0.0f, 1.0f));
	}
	catch (char* eText) {
		return false;
	}
	return true;
}

void MyApplication::Finalize() {
	if (this->param.textureList) {
		delete this->param.textureList;
		this->param.textureList = NULL;
	}
	if (this->param.shaderList) {
		delete this->param.shaderList;
		this->param.shaderList = NULL;
	}
	if (this->param.lightList) {
		delete this->param.lightList;
		this->param.lightList = NULL;
	}
	if (this->param.input) {
		delete this->param.input;
		this->param.input = NULL;
	}
	if (this->param.physicsSystem) {
		delete this->param.physicsSystem;
		this->param.physicsSystem = NULL;
	}
	if (this->model) {
		delete this->model;
		this->model = NULL;
	}
	if (this->player) {
		this->player->Finalize();
		delete this->player;
		this->player = NULL;
	}
	if (this->mapModel) {
		delete this->mapModel;
		this->mapModel = NULL;
	}
	if (this->camera) {
		delete this->camera;
		this->camera = NULL;
	}
	if (this->map) {
		delete this->map;
		this->map = NULL;
	}

	if (this->buffer) {
		delete this->buffer;
	}
	if (this->square) {
		delete this->square;
	}
}



void MyApplication::Run() {

	this->param.input->Run();

	this->param.camera = this->camera;

	this->player->Run(this->param);

	this->param.physicsSystem->Run();

	this->model->Run();
}

void MyApplication::Draw() {
	glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glViewport(0, 0, this->param.screenWidth, this->param.screenHeight);
	//MatrixPerspectiveLH(this->projectionMat, this->param.screenWidth, this->param.screenHeight, 10.0f, 100.0f, this->param.screenFov);
	this->projectionMat = this->camera->GetProjectionMatrix();
	DrawPass0();
}

////////
//private
////

//とりあえず物理エンジンのコード（初期化）
void MyApplication::bulletInitialize() {
	//エンジンを初期化
	this->param.physicsSystem = new BulletPhysics;

	for (int i = 0; i < 1; ++i)
	{
		btCollisionShape* shape = this->param.physicsSystem->CreateSphereShape(3.0f);
		btRigidBody* rigid = this->param.physicsSystem->CreateRigidBody(shape, 1.0f, 1, 1, btVector3(4.0f * i, 20.0f, 0.0f));
	}
	this->map->setCollisionWorld(this->param.physicsSystem);
}

//描画パス
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
	rot = AngleAxisf(DegToRad(0.0f), Vector3f::UnitX());
	mat = trans * rot * scale;
	shader = this->param.shaderList->UseShader("skin");
	this->param.lightList->SetAmbient("ambient", shader);
	this->param.lightList->SetAmbient("directional", shader);
	shader->SetVertexShaderSubroutine("NotSkinning");
	shader->SetMatrix(projectionMat * view * mat.matrix());
	shader->SetWorldMatrix(mat.matrix());
	this->mapModel->Draw(param, "skin");


	this->player->Draw(this->param);


	Matrix3f cameraInv = this->camera->GetCameraMatrix().block(0, 0, 3, 3);
	world = Matrix4f::Identity();
	trans = Translation<float, 3>(0, 0.0f, 0.0f);
	scale = DiagonalMatrix<float, 3>(1.0f, 1.0f, 1.0f);
	rot = AngleAxisf(DegToRad(0.0f), Vector3f::UnitX());
	mat = trans * cameraInv * rot * scale;
	shader = this->param.shaderList->UseShader("static");
	this->model->SetMatrix(mat.matrix(), view, projectionMat);

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	this->model->Draw(this->param);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	//デバッグ用コリジョン描画
	world = Matrix4f::Identity();
	trans = Translation<float, 3>(0, 0, 0);
	scale = DiagonalMatrix<float, 3>(1.0f, 1.0f, 1.0f);
	rot = AngleAxisf(DegToRad(0.0f), Vector3f::UnitX());
	mat = trans * rot * scale;
	//this->param.physicsSystem->DebugDraw(this->param.shaderList->GetShader("simple"), mat.matrix(), view, projectionMat);
}
void MyApplication::DrawPass1() {
}
void MyApplication::DrawPass2() {

}
