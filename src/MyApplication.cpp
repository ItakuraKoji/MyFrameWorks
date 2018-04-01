#include"MyApplication.h"
#include<Windows.h>

K_Math::Vector3 pos(0.0f, 10.6f, 0.0f);
float rotation = 0.0f;
float v = 0.0f, g = 0.03f;
float camerarotH, camerarotV;



MyApplication::MyApplication() {
}

MyApplication::~MyApplication() {
	Finalize();
}

unsigned MemoryUsageMegaBytes(void)
{
	MEMORYSTATUSEX m = { sizeof m };
	GlobalMemoryStatusEx(&m);
	return (unsigned)(((512 * 1024) + (m.ullTotalVirtual - m.ullAvailVirtual)) / (1024 * 1024));
}
void testPrint() {
	printf("現在のメモリ使用量：%u MB\n", MemoryUsageMegaBytes());
	system("pause");
}

bool MyApplication::Initialize(GameParameters* param, int width, int height) {
	try {
		//Parameters;
		this->param = param;
		this->param->GetFontRenderer()->LoadFont("onryou", "onryou.TTF");

		K_Graphics::ShaderList* shaderList = this->param->GetShaderList();
		shaderList->LoadVertexShader("Shader/VertexShader.vs");
		shaderList->LoadVertexShader("Shader/ShadowMapping.vs");
		shaderList->LoadVertexShader("Shader/SpriteShader.vs");
		shaderList->LoadVertexShader("Shader/SimpleShader.vs");
		shaderList->LoadFragmentShader("Shader/TextureSampler.ps");
		shaderList->LoadFragmentShader("Shader/DepthShader.ps");
		shaderList->LoadFragmentShader("Shader/ShadowMapping.ps");
		shaderList->LoadFragmentShader("Shader/MultipleTexture.ps");
		shaderList->LoadFragmentShader("Shader/SpriteShader.ps");
		shaderList->LoadFragmentShader("Shader/SimpleShader.ps");

		shaderList->CreateShaderProgram("standard", "Shader/VertexShader.vs", "Shader/TextureSampler.ps");
		shaderList->CreateShaderProgram("depth", "Shader/VertexShader.vs", "Shader/DepthShader.ps");
		shaderList->CreateShaderProgram("shadowMap", "Shader/ShadowMapping.vs", "Shader/ShadowMapping.ps");
		shaderList->CreateShaderProgram("shadow", "Shader/VertexShader.vs", "Shader/MultipleTexture.ps");
		shaderList->CreateShaderProgram("sprite", "Shader/SpriteShader.vs", "Shader/SpriteShader.ps");
		shaderList->CreateShaderProgram("simple", "Shader/SimpleShader.vs", "Shader/SimpleShader.ps");

		K_Graphics::CameraList* cameraList = this->param->GetCameraList();
		cameraList->AddPerspectiveCamera("mainCamera", K_Math::Vector3(0, 0, -1), K_Math::Vector3(0, 0, 0), this->param->windowWidth, this->param->windowHeight, 0.1f, 2000.0f, K_Math::DegToRad(45.0f));
		cameraList->AddOrthoCamera      ("lightCamera", K_Math::Vector3(0, 50, -50), K_Math::Vector3(0, 0, 0), 100.0f, 100.0f, 100.0f, 1000.0f);
		cameraList->AddOrthoCamera      ("2DCamera", K_Math::Vector3(0, 0, -1), K_Math::Vector3(0, 0, 0), this->param->windowWidth, this->param->windowHeight, 10.0f, 500.0f);

		this->frameBuffer = new K_Graphics::FrameBufferList(this->param->GetTextureList());
		this->frameBuffer->CreateFrameBuffer("result", this->param->windowWidth, this->param->windowHeight);
		this->frameBuffer->CreateFrameBuffer("geometry", this->param->windowWidth, this->param->windowHeight);
		this->frameBuffer->CreateFrameBuffer("shadowMap", "geometry", this->param->windowWidth, this->param->windowHeight);
		this->frameBuffer->CreateFrameBuffer("effect", "geometry", this->param->windowWidth, this->param->windowHeight);
		this->frameBuffer->CreateFrameBuffer("lightDepth", 2048, 2048);

		K_Graphics::ModelDataFactory factory;
		this->square    = new K_Graphics::MeshObject(new K_Graphics::MeshModel(factory.CreateSquareModel(this->param->windowWidth, this->param->windowHeight, this->param->GetTextureList()->GetTexture("frameBuffer"))));
		this->skinModel = new K_Graphics::MeshObject(new K_Graphics::MeshModel(factory.LoadFBXModel("kaminariNewWear.fbx", this->param->GetTextureList())));

		//this->skinModel = new MeshObject(new MeshModel(factory.LoadFBXModel("kaminariChan.fbx", this->param)));
		this->mapModel  = new K_Graphics::MeshObject(new K_Graphics::MeshModel(factory.LoadFBXModel("TestStage.fbx", this->param->GetTextureList())));

		this->player = new Player;
		this->player->SetDrawModel(this->skinModel);
		this->player->Initialize(this->param);
		this->player->SetCameraMan(this->param->GetCameraList()->GetCamera("mainCamera"));
		this->player->SetPosition(0.0f, 30.0f, -600.0f);

		this->map = new K_Physics::MapPolygon;
		this->map->LoadModel("TestStage.fbx");
		this->map->setCollisionWorld(this->param->GetPhysics());

		this->mapObj = new StaticObject;
		this->mapObj->SetDrawModel(this->mapModel);
		this->mapObj->Initialize(this->param);
		this->mapObj->SetMapCollision(this->map);

		K_Graphics::LightList* lightList = this->param->GetLightList();
		lightList->AddAmbient("ambient", 0.4f, K_Math::Vector4(0.5f, 1.0f, 1.0f, 1.0f));
		lightList->AddDirectional("directional", 1.0f, K_Math::Vector4(0.5f, 1.0f, 1.0f, 1.0f), K_Math::Vector3(0.0f, -1.0f, 1.0f));

		this->param->GetAudioList()->CreateSource("se", "hono.wav", K_Audio::SoundSource::LoadMode::AllRead);
		this->param->GetAudioList()->CreateSource("bgm", "Banbado.ogg", K_Audio::SoundSource::LoadMode::Streaming);
		this->param->GetAudioList()->GetSource("bgm")->SetVolume(0.1f);
		//this->param->GetAudioList()->GetSource("bgm")->Play(true);

		this->param->GetEffects()->SetMatrix(this->param->GetCameraList()->GetCamera("mainCamera"));
		this->param->GetEffects()->AddEffectSource("test", "test3.efk");

		this->testSprite = new K_Graphics::SpriteObject(nullptr);
		
	}
	catch (std::string& errorMessage) {
		std::cout << errorMessage << std::endl;
		system("pause");
		return false;
	}
	return true;
}

void MyApplication::Finalize() {

	if (this->player != nullptr) {
		this->player->Finalize();
		delete this->player;
		this->player = nullptr;
	}
	if (this->mapObj != nullptr) {
		this->mapObj->Finalize();
		delete this->mapObj;
		this->mapObj = nullptr;
	}
	if (this->square != nullptr) {
		delete this->square;
		this->square = nullptr;
	}
	if (this->testSprite != nullptr) {
		delete this->testSprite;
		this->testSprite = nullptr;
	}

	if (this->frameBuffer != nullptr) {
		delete this->frameBuffer;
		this->frameBuffer = nullptr;
	}
}

void MyApplication::Run() {
	if (glGetError() != GL_NO_ERROR) {
		printf("%s\n", glewGetErrorString(glGetError()));
	}

	this->player->Run(this->param);
	this->param->Run();
}

void MyApplication::Draw() {
	DrawPass0();
}

////////
//private
////



//描画パス
void MyApplication::DrawPass0() {
	//深度マップ
	this->frameBuffer->BeginDraw("lightDepth", 1.0f, 1.0f, 1.0f);

	//深度描画
	param->UseShader("depth");
	this->param->UseCamera("lightCamera");
	this->param->currentCamera->SetTarget(this->player->GetPosition().x(), this->player->GetPosition().y(), this->player->GetPosition().z());
	this->param->currentCamera->SetPosition(300.0f, K_Math::Vector3(0.0f, 1.0f, -1.0f));
	this->param->currentCamera->Draw();

	param->currentShader->SetFragmentShaderSubroutine("NoneDiffuse");
	this->player->Draw(this->param);

	param->currentShader->SetFragmentShaderSubroutine("NoneDiffuse");
	this->mapObj->Draw(this->param);
	this->frameBuffer->EndDraw();

	//シャドウのみ描画
	{
		this->frameBuffer->BeginDraw("shadowMap", 1.0f, 1.0f, 1.0f);

		this->param->UseCamera("mainCamera");
		this->param->currentCamera->Draw();

		param->UseShader("shadowMap");
		param->currentShader->SetFragmentShaderSubroutine("NoneDiffuse");
		param->UseCamera("lightCamera");
		K_Math::Matrix4x4 matVP = param->currentCamera->GetProjectionMatrix() * param->currentCamera->GetViewMatrix();
		param->currentShader->SetTexture("depthMap", 2, param->GetTextureList()->GetTexture("lightDepth")->GetTextureID());
		param->currentShader->SetValue("lightMatrixVP", matVP);

		param->UseCamera("mainCamera");

		//プレイヤー
		this->param->currentShader->SetValue("addVisible", 0.0f);
		this->player->Draw(this->param);
		//マップ
		this->param->currentShader->SetValue("addVisible", 0.0f);
		this->mapObj->Draw(this->param);
		this->frameBuffer->EndDraw();
	}

	//通常描画
	{
		this->frameBuffer->BeginDraw("geometry", 0.5f, 0.7f, 0.9f);

		this->param->UseCamera("mainCamera");
		this->param->currentCamera->Draw();

		param->UseShader("standard");
		param->currentShader->SetFragmentShaderSubroutine("CalcDiffuse");
		param->currentShader->SetFragmentShaderSubroutine("CalcSpecular");
		param->UseAmbient("ambient");
		param->UseDirectional("directional");
		param->UseCamera("mainCamera");

		//プレイヤー
		this->player->Draw(this->param);
		//マップ
		this->mapObj->Draw(this->param);

		//デバッグ用コリジョン描画
		K_Math::Matrix4x4 world = K_Math::Matrix4x4::Identity();
		//this->param->GetPhysics()->DebugDraw(this->param->GetShaderList()->GetShader("simple"), this->param->currentCamera, world);
		//文字描画
		//glDepthMask(GL_FALSE);
		param->UseShader("sprite");
		this->param->GetFontRenderer()->Draw(this->param->currentCamera, this->param->currentShader);
		//glDepthMask(GL_TRUE);
	}


	//エフェクト（深度使いまわし）
	{
		this->frameBuffer->BeginDraw("effect", 0.0f, 0.0f, 0.0f, true);

		param->UseShader("standard");
		this->param->UseCamera("mainCamera");
		this->param->currentCamera->Draw();

		//エフェクト描画
		this->param->GetEffects()->Draw();

		this->frameBuffer->EndDraw();
	}

	//レンダリング結果のテクスチャ描画
	this->frameBuffer->BeginDraw(this->param->windowWidth, this->param->windowHeight, 0.0f, 0.0f, 0.0f);
	param->UseShader("shadow");
	this->param->UseCamera("2DCamera");
	param->currentShader->SetFragmentShaderSubroutine("NoneDiffuse");
	param->currentShader->SetTexture("geometry", 1, param->GetTextureList()->GetTexture("geometry")->GetTextureID());
	param->currentShader->SetTexture("shadow", 2, param->GetTextureList()->GetTexture("shadowMap")->GetTextureID());
	param->currentShader->SetTexture("effect", 3, param->GetTextureList()->GetTexture("effect")->GetTextureID());
	//this->square->Draw(this->param, Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(-1, 1, 1));

	testSprite->Draw2D(this->param->currentCamera, this->param->currentShader, K_Math::Box2D(0, 0, this->param->windowWidth, this->param->windowHeight), K_Math::Box2D(0, 0, this->param->windowWidth, this->param->windowHeight), 0.0f);

	param->UseShader("sprite");
	testSprite->SetTexture(param->GetTextureList()->GetTexture("lightDepth"));
	testSprite->Draw2D(this->param->currentCamera, this->param->currentShader, K_Math::Box2D(0, 0, 2048, 2048), K_Math::Box2D(0, 0, 230, 230), 0.0f);

}

void MyApplication::DrawPass1() {

}

void MyApplication::DrawPass2() {
}
