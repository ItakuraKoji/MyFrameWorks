#include"SystemClass.h"
#include<iostream>

//メモリリーク検知用
#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SystemClass *system = new SystemClass;
	if (system->Initialize()) {
		system->Run();
	}
	delete system;
}