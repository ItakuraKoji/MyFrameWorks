
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")


#include"SystemClass.h"
#include<iostream>

//���������[�N���m�p
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
		//�A�v���V�X�e��������
		systemClass = new SystemClass(1920 / 2, 1080 / 2, false);
	
		//�A�v��������
		app = new MyApplication;
		if (!app->Initialize(systemClass->GetParameters(), systemClass->GetWindowWidth(), systemClass->GetWindowHeight())) {
			delete app;
			delete systemClass;
			throw;
		}

		//���ۂ̃Q�[������
		while (!systemClass->WindowClosed())
		{
			systemClass->ProcessSystem();
			//�E�B���h�E�̓t�H�[�J�X����Ă���Ƃ������Q�[���̏���
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