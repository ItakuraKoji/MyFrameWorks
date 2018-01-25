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
		shaderList->LoadVertexShader("Shader/VertexShader.vs");
		shaderList->LoadVertexShader("Shader/ShadowMapping.vs");
		shaderList->LoadFragmentShader("Shader/TextureSampler.ps");
		shaderList->LoadFragmentShader("Shader/DepthShader.ps");
		shaderList->LoadFragmentShader("Shader/ShadowMapping.ps");
		shaderList->LoadFragmentShader("Shader/MultipleTexture.ps");
		if (!shaderList->CreateShaderProgram("standard", "Shader/VertexShader.vs", "Shader/TextureSampler.ps")) {
			return false;
		}
		if (!shaderList->CreateShaderProgram("depth", "Shader/VertexShader.vs", "Shader/DepthShader.ps")) {
			return false;
		}
		if (!shaderList->CreateShaderProgram("shadowMap", "Shader/ShadowMapping.vs", "Shader/ShadowMapping.ps")) {
			return false;
		}
		if (!shaderList->CreateShaderProgram("shadow", "Shader/VertexShader.vs", "Shader/MultipleTexture.ps")) {
			return false;
		}

		shaderList->LoadVertexShader("Shader/StaticShader.vs");
		shaderList->LoadVertexShader("Shader/SimpleShader.vs");

		shaderList->LoadFragmentShader("Shader/StaticShader.ps");
		shaderList->LoadFragmentShader("Shader/SimpleShader.ps");

		if (!shaderList->CreateShaderProgram("static", "Shader/StaticShader.vs", "Shader/StaticShader.ps")) {
			return false;
		}
		if (!shaderList->CreateShaderProgram("simple", "Shader/SimpleShader.vs", "Shader/SimpleShader.ps")) {
			return false;
		}

		CameraList* cameraList = this->param->GetCameraList();
		cameraList->AddPerspectiveCamera("mainCamera", Vector3f(0, 0, -1), Vector3f(0, 0, 0), this->param->screenWidth, this->param->screenHeight, 0.1f, 1000.0f, DegToRad(45.0f));
		cameraList->AddOrthoCamera      ("lightCamera", Vector3f(0, 50, -50), Vector3f(0, 0, 0), 230.0f, 230.0f, 10.0f, 500.0f);
		cameraList->AddOrthoCamera      ("2DCamera", Vector3f(0, 0, -1), Vector3f(0, 0, 0), this->param->screenWidth, this->param->screenHeight, 10.0f, 500.0f);


		this->buffer = new Framebuffer(this->param->GetTextureList(), "frameBuffer", this->param->screenWidth, this->param->screenHeight);
		this->shadowMap = new Framebuffer(this->param->GetTextureList(), "shadowMap", this->param->screenWidth, this->param->screenHeight);
		this->lightDepthMap = new Framebuffer(this->param->GetTextureList(), "lightDepth", 2048, 2048);

		ModelDataFactory factory;
		this->square    = new MeshObject(new MeshModel(factory.CreateSquareModel(this->param->screenWidth, this->param->screenHeight, "frameBuffer", this->param)));
		this->mapModel  = new MeshObject(new MeshModel(factory.LoadFBXModel("Map.fbx", this->param)));
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


		LightList* lightList = this->param->GetLightList();
		lightList->AddAmbient("ambient", 0.4f, Vector4f(0.5f, 1.0f, 1.0f, 1.0f));
		lightList->AddDirectional("directional", 1.0f, Vector4f(0.7f, 1.0f, 1.0f, 1.0f), Vector3f(0.0f, -1.0f, 1.0f));

		this->param->GetAudioList()->CreateSource("bgm", "PerituneMaterial_Prairie_loop.ogg");
		this->param->GetAudioList()->GetSource("bgm")->SetVolume(0.5f);
		this->param->GetAudioList()->GetSource("bgm")->Play(true);

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
	if (this->shadowMap) {
		delete this->shadowMap;
	}
	if (this->lightDepthMap) {
		delete this->lightDepthMap;
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
	DrawPass0();
	DrawPass1();
}

////////
//private
////

//描画パス
void MyApplication::DrawPass0() {
	this->lightDepthMap->Bind();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glViewport(0, 0, this->lightDepthMap->GetWidth(), this->lightDepthMap->GetHeight());

	this->param->UseCamera("lightCamera");


	//深度描画
	param->UseShader("depth");

	param->currentShader->SetVertexShaderSubroutine("CalcBoneMat");
	param->currentShader->SetFragmentShaderSubroutine("CalcLight");
	this->player->Draw(this->param);


	param->currentShader->SetVertexShaderSubroutine("NotSkinning");
	param->currentShader->SetFragmentShaderSubroutine("CalcLight");
	this->mapObj->Draw(this->param);
	this->lightDepthMap->UnBind();
}

void MyApplication::DrawPass1() {
	this->shadowMap->Bind();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
	glViewport(0, 0, this->shadowMap->GetWidth(), this->shadowMap->GetHeight());

	this->param->UseCamera("mainCamera");
	this->param->currentCamera->Draw();



	param->UseShader("shadowMap");
	param->UseCamera("lightCamera");
	Matrix4f matVP = param->currentCamera->GetProjectionMatrix() * param->currentCamera->GetViewMatrix();
	param->currentShader->SetTexture("depthMap", 2, param->GetTextureList()->GetTexture("lightDepth")->GetTextureID());
	param->currentShader->SetValue("lightMatrixVP", matVP);

	param->UseCamera("mainCamera");

	//プレイヤー
	param->currentShader->SetVertexShaderSubroutine("CalcBoneMat");
	param->currentShader->SetFragmentShaderSubroutine("CalcLight");
	this->player->Draw(this->param);
	//マップ
	param->currentShader->SetVertexShaderSubroutine("NotSkinning");
	param->currentShader->SetFragmentShaderSubroutine("CalcLight");
	this->mapObj->Draw(this->param);

	this->shadowMap->UnBind();

	this->buffer->Bind();
	glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glViewport(0, 0, this->param->screenWidth, this->param->screenHeight);
	param->UseShader("standard");
	param->UseAmbient("ambient");
	param->UseDirectional("directional");

	param->UseCamera("mainCamera");

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

	glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glViewport(0, 0, this->param->screenWidth, this->param->screenHeight);
	param->UseShader("shadow");
	param->currentShader->SetVertexShaderSubroutine("NotSkinning");
	param->currentShader->SetFragmentShaderSubroutine("None");
	param->currentShader->SetTexture("sampler2", 2, param->GetTextureList()->GetTexture("shadowMap")->GetTextureID());
	this->square->SetTexture("frameBuffer", 0, 0);
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
