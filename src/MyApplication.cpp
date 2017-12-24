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
		this->param = new GameParameters;
		this->param->Initialize(width, height);

		ShaderList* shaderList = this->param->GetShaderList();
		shaderList->AddVertexShader("Shader/VertexShader.vs");
		shaderList->AddPixelShader("Shader/TextureSampler.ps");
		shaderList->AddPixelShader("Shader/DepthShader.ps");
		if (!shaderList->Add("standard", "Shader/VertexShader.vs", "Shader/TextureSampler.ps")) {
			return false;
		}
		if (!shaderList->Add("depth", "Shader/VertexShader.vs", "Shader/DepthShader.ps")) {
			return false;
		}

		shaderList->AddVertexShader("Shader/StaticShader.vs");
		shaderList->AddVertexShader("Shader/SimpleShader.vs");

		shaderList->AddPixelShader("Shader/StaticShader.ps");
		shaderList->AddPixelShader("Shader/SimpleShader.ps");

		if (!shaderList->Add("static", "Shader/StaticShader.vs", "Shader/StaticShader.ps")) {
			return false;
		}
		if (!shaderList->Add("simple", "Shader/SimpleShader.vs", "Shader/SimpleShader.ps")) {
			return false;
		}

		CameraList* cameraList = this->param->GetCameraList();
		cameraList->AddPerspectiveCamera("mainCamera", this->param->screenWidth, this->param->screenHeight, 0.1f, 1000.0f, DegToRad(45.0f));
		cameraList->AddOrthoCamera      ("lightCamera", 230.0f, 230.0f, 10.0f, 100.0f);
		cameraList->AddOrthoCamera      ("2DCamera", this->param->screenWidth, this->param->screenHeight, 10.0f, 100.0f);


		ModelDataFactory factory;
		this->mapModel = new MeshObject(new MeshModel(factory.LoadFBXModel("Map.fbx", this->param)));



		this->skinModel = new MeshObject(new MeshModel(factory.LoadFBXModel("KaminariChan.fbx", this->param)));

		this->player = new Player();
		this->player->SetDrawModel(this->skinModel);
		this->player->SetPosition(0.0f, 10.6f, 0.0f);
		this->player->Initialize(this->param);
		this->player->SetCameraMan(this->param->GetCameraList()->GetCamera("mainCamera"));

		this->map = new MapPolygon;
		this->map->LoadModel("Map.fbx");
		this->map->setCollisionWorld(this->param->GetPhysics());

		this->mapObj = new StaticObject;
		this->mapObj->SetDrawModel(this->mapModel);
		this->mapObj->Initialize(this->param);
		this->mapObj->SetMapCollision(this->map);

		//this->model = new Emitter;
		//if (!this->model->Initialize(this->param)) {
		//	return false;
		//}

		//
		this->buffer = new Framebuffer(this->param->GetTextureList(), "frameBuffer", this->param->screenWidth, this->param->screenWidth);
		this->square = new MeshObject(new MeshModel(factory.CreateSquareModel(this->param->screenWidth, this->param->screenHeight, "frameBuffer", this->param)));

		LightList* lightList = this->param->GetLightList();
		lightList->AddAmbient("ambient", 0.4f, Vector4f(0.5f, 1.0f, 1.0f, 1.0f));
		lightList->AddDirectional("directional", 1.0f, Vector4f(0.7f, 1.0f, 1.0f, 1.0f), Vector3f(0.0f, -1.0f, 1.0f));



	}
	catch (char* eText) {
		return false;
	}
	return true;
}

void MyApplication::Finalize() {
	if (this->param) {
		this->param->Finalize();
		delete this->param;
		this->param = NULL;
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

	if (this->buffer) {
		delete this->buffer;
	}
	if (this->square) {
		delete this->square;
	}
}



void MyApplication::Run() {

	this->param->Run();

	this->player->Run(this->param);

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
	this->param->UseCamera("lightCamera");
	this->param->currentCamera->Draw();

	//深度描画
	param->UseShader("depth");
	param->UseAmbient("ambient");
	param->UseDirectional("directional");

	param->currentShader->SetVertexShaderSubroutine("CalcBoneMat");
	param->currentShader->SetFragmentShaderSubroutine("CalcLight");
	this->player->Draw(this->param);


	param->currentShader->SetVertexShaderSubroutine("NotSkinning");
	param->currentShader->SetFragmentShaderSubroutine("CalcLight");
	this->mapObj->Draw(this->param);

}
void MyApplication::DrawPass1() {
	this->buffer->Bind();
	glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
	glViewport(0, 0, this->param->screenWidth, this->param->screenHeight);

	this->param->UseCamera("mainCamera");
	this->param->currentCamera->Draw();

	param->UseShader("standard");
	param->UseAmbient("ambient");
	param->UseDirectional("directional");

	//プレイヤー
	param->currentShader->SetVertexShaderSubroutine("CalcBoneMat");
	param->currentShader->SetFragmentShaderSubroutine("CalcLight");
	this->player->Draw(this->param);
	//マップ
	param->currentShader->SetVertexShaderSubroutine("NotSkinning");
	param->currentShader->SetFragmentShaderSubroutine("CalcLight");
	this->mapObj->Draw(this->param);

	this->buffer->UnBind();

	//レンダリング結果のテクスチャ描画
	this->param->UseCamera("2DCamera");
	this->param->currentCamera->SetPosition(0, 0, -1);
	this->param->currentCamera->SetTarget(0, 0, 0);
	this->param->currentCamera->Draw();

	glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glViewport(0, 0, this->param->screenWidth, this->param->screenHeight);
	param->currentShader->SetVertexShaderSubroutine("NotSkinning");
	param->currentShader->SetFragmentShaderSubroutine("None");
	this->square->Draw(this->param, Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(-1, 1, 1));

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
	Matrix4f projection = this->param->currentCamera->GetProjectionMatrix();
	Matrix4f view = this->param->currentCamera->GetViewMatrix();
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
