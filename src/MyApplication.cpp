#include"MyApplication.h"
#include<Windows.h>

Vector3f pos(0.0f, 10.6f, 0.0f);
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

bool MyApplication::Initialize(GLFWwindow* window, int width, int height) {
	try {
		//Parameters;
		this->param = new GameParameters(window, width, height);
		this->param->GetFontRenderer()->LoadFont("onryou", "onryou.TTF");

		ShaderList* shaderList = this->param->GetShaderList();
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

		CameraList* cameraList = this->param->GetCameraList();
		cameraList->AddPerspectiveCamera("mainCamera", Vector3f(0, 0, -1), Vector3f(0, 0, 0), this->param->screenWidth, this->param->screenHeight, 0.1f, 2000.0f, M::DegToRad(45.0f));
		cameraList->AddOrthoCamera      ("lightCamera", Vector3f(0, 50, -50), Vector3f(0, 0, 0), 230.0f, 230.0f, 10.0f, 500.0f);
		cameraList->AddOrthoCamera      ("2DCamera", Vector3f(0, 0, -1), Vector3f(0, 0, 0), this->param->screenWidth, this->param->screenHeight, 10.0f, 500.0f);

		this->frameBuffer = new FrameBufferList(this->param->GetTextureList());
		this->frameBuffer->CreateFrameBuffer("result", this->param->screenWidth, this->param->screenHeight);
		this->frameBuffer->CreateFrameBuffer("geometry", this->param->screenWidth, this->param->screenHeight);
		this->frameBuffer->CreateFrameBuffer("shadowMap", "geometry", this->param->screenWidth, this->param->screenHeight);
		this->frameBuffer->CreateFrameBuffer("effect", "geometry", this->param->screenWidth, this->param->screenHeight);
		this->frameBuffer->CreateFrameBuffer("lightDepth", 2048, 2048);

		ModelDataFactory factory;
		this->square    = new MeshObject(new MeshModel(factory.CreateSquareModel(this->param->screenWidth, this->param->screenHeight, this->param->GetTextureList()->GetTexture("frameBuffer"))));
		this->skinModel = new MeshObject(new MeshModel(factory.LoadFBXModel("kaminariNewWear.fbx", this->param->GetTextureList())));

		//this->skinModel = new MeshObject(new MeshModel(factory.LoadFBXModel("kaminariChan.fbx", this->param)));
		this->mapModel  = new MeshObject(new MeshModel(factory.LoadFBXModel("TestStage.fbx", this->param->GetTextureList())));

		this->player = new Player;
		this->player->SetDrawModel(this->skinModel);
		this->player->Initialize(this->param);
		this->player->SetCameraMan(this->param->GetCameraList()->GetCamera("mainCamera"));
		this->player->SetPosition(0.0f, 30.0f, -600.0f);

		this->map = new MapPolygon;
		this->map->LoadModel("TestStage.fbx");
		this->map->setCollisionWorld(this->param->GetPhysics());

		this->mapObj = new StaticObject;
		this->mapObj->SetDrawModel(this->mapModel);
		this->mapObj->Initialize(this->param);
		this->mapObj->SetMapCollision(this->map);

		LightList* lightList = this->param->GetLightList();
		lightList->AddAmbient("ambient", 0.4f, Vector4f(0.5f, 1.0f, 1.0f, 1.0f));
		lightList->AddDirectional("directional", 1.0f, Vector4f(0.5f, 1.0f, 1.0f, 1.0f), Vector3f(0.0f, -1.0f, 1.0f));

		this->param->GetAudioList()->CreateSource("se", "hono.wav", SoundSource::LoadMode::AllRead);
		this->param->GetAudioList()->CreateSource("bgm", "Banbado.ogg", SoundSource::LoadMode::Streaming);
		this->param->GetAudioList()->GetSource("bgm")->SetVolume(0.1f);
		//this->param->GetAudioList()->GetSource("bgm")->Play(true);

		this->param->GetEffects()->SetMatrix(this->param->GetCameraList()->GetCamera("mainCamera"));
		this->param->GetEffects()->AddEffectSource("test", "test3.efk");

		this->testSprite = new SpriteObject(nullptr);
		
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

	if (this->param != nullptr) {
		this->param->Finalize();
		delete this->param;
		this->param = nullptr;
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
		Matrix4f matVP = param->currentCamera->GetProjectionMatrix() * param->currentCamera->GetViewMatrix();
		param->currentShader->SetTexture("depthMap", 2, param->GetTextureList()->GetTexture("lightDepth")->GetTextureID());
		param->currentShader->SetValue("lightMatrixVP", matVP);

		param->UseCamera("mainCamera");

		//プレイヤー
		this->param->currentShader->SetValue("addVisible", 100.0f);
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
		Matrix4f world = Matrix4f::Identity();
		//this->param->GetPhysics()->DebugDraw(this->param->GetShaderList()->GetShader("simple"), this->param->currentCamera, world);
		param->UseShader("sprite");
		this->param->GetFontRenderer()->Draw(this->param->currentCamera, this->param->currentShader);
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
	this->frameBuffer->BeginDraw(this->param->screenWidth, this->param->screenHeight, 0.0f, 0.0f, 0.0f);
	param->UseShader("shadow");
	this->param->UseCamera("2DCamera");
	param->currentShader->SetFragmentShaderSubroutine("NoneDiffuse");
	param->currentShader->SetTexture("geometry", 1, param->GetTextureList()->GetTexture("geometry")->GetTextureID());
	param->currentShader->SetTexture("shadow", 2, param->GetTextureList()->GetTexture("shadowMap")->GetTextureID());
	param->currentShader->SetTexture("effect", 3, param->GetTextureList()->GetTexture("effect")->GetTextureID());
	//this->square->Draw(this->param, Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(-1, 1, 1));

	testSprite->Draw2D(this->param->currentCamera, this->param->currentShader, M::Box2D(0, 0, this->param->screenWidth, this->param->screenHeight), M::Box2D(0, 0, this->param->screenWidth, this->param->screenHeight));


}

void MyApplication::DrawPass1() {

}

void MyApplication::DrawPass2() {
}
