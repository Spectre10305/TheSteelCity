#include "Engine.h"
#include "../utils/Log.h"
#include "../utils/Time.h"


// =================================================


bool nothing::Engine::Init()
{

	nothing::LogInfo("Initializing engine...");


	nothing::StartTimer();


	if (!windowManager_.Init())
	{

		nothing::LogInfo("Can't initialize WindowManager, shutting down everything...");
		return false;

	}


	renderManager_.Init();


	double timeToInitialize = nothing::ElapsedMS();
	nothing::LogInfo("Took: " + std::to_string(timeToInitialize) + " MS to initialize engine");


	return true;

}


// =================================================


void nothing::Engine::Run()
{

	nothing::LogInfo("Running...");


	while (isRunning)
	{

		double deltaTime = nothing::CalcDeltaTime();


		SDL_Event event;
		HandleEvents(event);


		renderManager_.Update();


		SDL_GL_SwapWindow(windowManager_.GetWindowPtr());


		windowManager_.SetWindowTitle(("Nothing Engine DEMO - DeltaTime: " + std::to_string(deltaTime)).c_str());

	}

}


// =================================================


void nothing::Engine::Shutdown()
{

	nothing::LogInfo("Shutting down...");


	renderManager_.Shutdown();
	windowManager_.Shutdown();

}


// =================================================


void nothing::Engine::HandleEvents(SDL_Event& event)
{

	// Eventi SDL (finestra, tastiera, mouse)
	while (SDL_PollEvent(&event))
	{

		switch (event.type)
		{

		case SDL_EVENT_WINDOW_RESIZED:
			int w, h;
			windowManager_.GetWindowSize(w, h);
			renderManager_.ResizeGLViewport(w, h);
			nothing::LogInfo("Window resized to: " + std::to_string(w) + "X" + std::to_string(h));
			break;


		case SDL_EVENT_QUIT:
			isRunning = false;
			break;
			

		default:
			break;

		}

	}

}


// =================================================