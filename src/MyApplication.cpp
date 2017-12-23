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
		this->param.physicsSystem = new BulletPhysics;
		this->param.gravity = 0.03f;
		this->param.screenWidth = width;
		this->param.screenHeight = height;
		this->param.screenFov = DegToRad(45.0f);
		this->param.input = new InputClass(0);
		this->param.textureList = new TextureList;
		this->param.shaderList = new ShaderList;
		this->param.shaderList->Initialize();

		this->param.shaderList->AddVertexShader("Shader/VertexShader.vs");
		this->param.shaderList->AddPixelShader("Shader/TextureSampler.ps");
		this->param.shaderList->AddPixelShader("Shader/DepthShader.ps");
		if (!this->param.shaderList->Add("standard", "Shader/VertexShader.vs", "Shader/TextureSampler.ps")) {
			return false;
		}
		if (!this->param.shaderList->Add("depth", "Shader/VertexShader.vs", "Shader/DepthShader.ps")) {
			return false;
		}

		this->param.shaderList->AddVertexShader("Shader/StaticShader.vs");
		this->param.shaderList->AddVertexShader("Shader/SimpleShader.vs");

		this->param.shaderList->AddPixelShader("Shader/StaticShader.ps");
		this->param.shaderList->AddPixelShader("Shader/SimpleShader.ps");



		if (!this->param.shaderList->Add("static", "Shader/StaticShader.vs", "Shader/StaticShader.ps")) {
			return false;
		}
		if (!this->param.shaderList->Add("simple", "Shader/SimpleShader.vs", "Shader/SimpleShader.ps")) {
			return false;
		}



		ModelDataFactory factory;
		this->mapModel = new MeshObject(new MeshModel(factory.LoadFBXModel("Map.fbx", this->param)));



		this->skinModel = new MeshObject(new MeshModel(factory.LoadFBXModel("KaminariChan.fbx", this->param)));
		this->camera = new CameraClass(ProjectionType::Perspective, this->param.screenWidth, this->param.screenHeight, 5.1f, 1000.0f, this->param.screenFov);
		this->lightCamera = new CameraClass(ProjectionType::Ortho, 230.0f, 230.0f, 10.0f, 100.0f, this->param.screenFov);
		this->lightCamera->SetPosition(3, 1, -1);
		this->lightCamera->SetTarget(3, 0, 0);
		this->lightCamera->Draw();

		this->player = new Player();
		this->player->SetDrawModel(this->skinModel);
		this->player->SetPosition(0.0f, 10.6f, 0.0f);
		this->player->Initialize(this->param);
		this->player->SetCameraMan(this->camera);

		this->map = new MapPolygon;
		this->map->LoadModel("Map.fbx");
		this->map->setCollisionWorld(this->param.physicsSystem);

		this->mapObj = new StaticObject;
		this->mapObj->SetDrawModel(this->mapModel);
		this->mapObj->Initialize(this->param);
		this->mapObj->SetMapCollision(this->map);

		//this->model = new Emitter;
		//if (!this->model->Initialize(this->param)) {
		//	return false;
		//}

		//
		this->buffer = new Framebuffer(this->param.textureList, "frameBuffer", this->param.screenWidth, this->param.screenWidth);
		this->square = new MeshObject(new MeshModel(factory.CreateSquareModel(this->param.screenWidth, this->param.screenHeight, "frameBuffer", this->param)));

		this->param.lightList = new LightList;
		this->param.lightList->AddAmbient("ambient", 0.4f, Vector4f(0.5f, 1.0f, 1.0f, 1.0f));
		this->param.lightList->AddDirectional("directional", 1.0f, Vector4f(0.7f, 1.0f, 1.0f, 1.0f), Vector3f(0.0f, -1.0f, 1.0f));



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
	//if (this->model) {
	//	delete this->model;
	//	this->model = NULL;
	//}
	if (this->player) {
		this->player->Finalize();
		delete this->player;
		this->player = NULL;
	}
	if (this->mapObj) {
		this->mapObj->Finalize();
		delete this->mapObj;
	}
	if (this->camera) {
		delete this->camera;
		this->camera = NULL;
	}
	if (this->lightCamera) {
		delete this->lightCamera;
		this->lightCamera = NULL;
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

	//this->model->Run();
}

void MyApplication::Draw() {
	//MatrixPerspectiveLH(this->projectionMat, this->param.screenWidth, this->param.screenHeight, 10.0f, 100.0f, this->param.screenFov);
	DrawPass1();
}

////////
//private
////

//描画パス
void MyApplication::DrawPass0() {
	this->camera->Draw();
	this->param.camera = lightCamera;

	//深度描画
	param.currentShader = param.shaderList->UseShader("depth");
	param.lightList->SetAmbient("ambient", param.currentShader);
	param.lightList->SetDirectional("directional", param.currentShader);

	param.currentShader->SetVertexShaderSubroutine("CalcBoneMat");
	param.currentShader->SetFragmentShaderSubroutine("CalcLight");
	this->player->Draw(this->param);


	param.currentShader->SetVertexShaderSubroutine("NotSkinning");
	param.currentShader->SetFragmentShaderSubroutine("CalcLight");
	this->mapObj->Draw(this->param);

}
void MyApplication::DrawPass1() {
	glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
	glViewport(0, 0, this->param.screenWidth, this->param.screenHeight);

	this->camera->Draw();
	this->param.camera = camera;


	param.currentShader = param.shaderList->UseShader("standard");
	param.lightList->SetAmbient("ambient", param.currentShader);
	param.lightList->SetDirectional("directional", param.currentShader);

	param.currentShader->SetVertexShaderSubroutine("CalcBoneMat");
	param.currentShader->SetFragmentShaderSubroutine("CalcLight");
	this->player->Draw(this->param);


	param.currentShader->SetVertexShaderSubroutine("NotSkinning");
	param.currentShader->SetFragmentShaderSubroutine("CalcLight");
	this->mapObj->Draw(this->param);


	//Matrix3f cameraInv = this->camera->GetCameraMatrix().block(0, 0, 3, 3);
	//world = Matrix4f::Identity();
	//trans = Translation<float, 3>(0, 0.0f, 0.0f);
	//scale = DiagonalMatrix<float, 3>(1.0f, 1.0f, 1.0f);
	//rot = AngleAxisf(DegToRad(0.0f), Vector3f::UnitX());
	//mat = trans * cameraInv * rot * scale;
	//shader = this->param.shaderList->UseShader("static");
	//this->model->SetMatrix(mat.matrix(), view, projectionMat);

	//glDepthMask(GL_FALSE);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	//this->model->Draw(this->param);
	//glDisable(GL_BLEND);
	//glDepthMask(GL_TRUE);





	//デバッグ用コリジョン描画
	Matrix4f projection = this->param.camera->GetProjectionMatrix();
	Matrix4f view = this->param.camera->GetViewMatrix();
	Matrix4f world;
	Translation<float, 3> trans;
	DiagonalMatrix<float, 3> scale;
	Quaternionf rot;
	Affine3f mat;
	world = Matrix4f::Identity();
	trans = Translation<float, 3>(0, 0, 0);
	scale = DiagonalMatrix<float, 3>(1.0f, 1.0f, 1.0f);
	rot = AngleAxisf(DegToRad(0.0f), Vector3f::UnitX());
	mat = trans * rot * scale;
	//this->param.physicsSystem->DebugDraw(this->param.shaderList->GetShader("simple"), mat.matrix(), view, projection);
}
void MyApplication::DrawPass2() {
}
