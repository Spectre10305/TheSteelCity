#include "core/Engine.h"
#include "utils/Log.h"


// Questo viene esportatato verso tsc.exe per avviare l'engine
extern "C" __declspec(dllexport)
void EngineMain(int args, char* argsv[])
{

	nothing::LaunchOptions launchOpts{};
	

	for (int i = 1; i < args; i++)
	{

		std::string arg = argsv[i];


		if (arg == "-map" && i + 1 < args)
		{

			launchOpts.mapOnLaunchName = argsv[++i];
			nothing::LogInfo("Launching with map: " + launchOpts.mapOnLaunchName);

		}


		if (arg == "-nosplash")
		{

			launchOpts.noSplash = true;

		}


		if (arg == "-devmode")
		{

			// Attiva la modalità sviluppo

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