
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

	try
	{
		SystemClass *systemClass = new SystemClass;
		if (systemClass->Initialize()) {
			systemClass->Run();
		}
		delete systemClass;

	}
	catch (const std::exception&)
	{
		system("pause");
	}
}