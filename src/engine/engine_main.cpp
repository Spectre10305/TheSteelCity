#include "core/Engine.h"
#include "utils/Log.h"


struct LaunchOptions
{

	int test1 = 0;
	int test2 = 0;
	int test3 = 0;

};


// Questo viene esportatato verso tsc.exe per avviare l'engine
extern "C" __declspec(dllexport)
void EngineMain(int args, char* argsv[])
{


	LaunchOptions launchOpts{};


	nothing::LogInfo("All arguments: " + std::to_string(args));


	for (size_t i = 0; i < args; i++)
	{

		nothing::LogInfo(argsv[i]);

	}


	nothing::Engine e;


	if (!e.Init())
	{
		nothing::LogError("Cannot initialize engine");
		return;
	}


	e.Run();
	e.Shutdown();

}