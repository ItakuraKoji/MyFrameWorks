
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")


#include"SystemClass.h"
#include<iostream>

//メモリリーク検知用
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SystemClass *systemClass = nullptr;
	MyApplication* app = nullptr;
	try
	{
		//アプリシステム初期化
		systemClass = new SystemClass(1920 / 2, 1080 / 2, false);
	
		//アプリ初期化
		app = new MyApplication;
		if (!app->Initialize(systemClass->GetParameters(), systemClass->GetWindowWidth(), systemClass->GetWindowHeight())) {
			delete app;
			delete systemClass;
			throw;
		}

		//実際のゲーム処理
		while (!systemClass->WindowClosed())
		{
			systemClass->ProcessSystem();
			//ウィンドウはフォーカスされているときだけゲームの処理
			if (!systemClass->WindowForcus()) {
				continue;
			}
			app->Run();
			app->Draw();
			systemClass->SwapBuffer();
		}
	}
	catch (const std::exception&)
	{
		system("pause");
	}

	if (app != nullptr) {
		delete app;
	}
	if (systemClass != nullptr) {
		delete systemClass;
	}
}