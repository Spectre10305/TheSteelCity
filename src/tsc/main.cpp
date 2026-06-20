#include <Windows.h>
#include <stdio.h>


// Utility tattica
void LogInfo(const char* message)
{

#ifdef _DEBUG


	printf(message);


#else


	MessageBoxA(NULL, message, "The Steel City", 0);


#endif

}


int main(int argc, char* argv[])
{

	HMODULE lib = LoadLibrary(L"engine.dll");


	if (!lib)
	{

		// Non si è caricata la libreria
		LogInfo("Can't load library 'engine.dll'");
		return -1;

	}


	using EngineMainFn = void(*)(int, char**);
	EngineMainFn emain = reinterpret_cast<EngineMainFn>(GetProcAddress(lib, "EngineMain"));


	if (!emain)
	{

		LogInfo("Can't start engine");
		return -1;

	}


	// Punto di ingresso engine
	emain(argc, argv);

	
	FreeLibrary(lib);


	return 0;

}