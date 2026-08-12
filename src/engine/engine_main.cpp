#include "core/Engine.h"
#include "utils/Log.h"


// Questo viene esportatato verso tsc.exe per avviare l'engine
extern "C" __declspec(dllexport)
void EngineMain(int args, char* argsv[])
{

	nothing::LaunchOptions launchOpts{};
	

	for (size_t i = 0; i < args; i++)
	{

		nothing::LogInfo(argsv[i]);


		if (std::strcmp(argsv[i], "-nosplash") == 0)
		{

			launchOpts.noSplash = true;

		}

	}


	nothing::Engine e;


	if (!e.Init(launchOpts))
	{
		nothing::LogError("Cannot initialize engine");
		return;
	}


	e.Run();
	e.Shutdown();

}