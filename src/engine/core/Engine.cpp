#include "Engine.h"
#include "../utils/Log.h"
#include <SDL3/SDL.h>


// =================================================


void nothing::Engine::Init()
{

	nothing::LogInfo("Initializing engine...");


	if (!SDL_Init(SDL_INIT_VIDEO))
	{

		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return;

	}


	_mainWindow = SDL_CreateWindow("Nothing Engine DEMO", 1240, 720, SDL_WINDOW_OPENGL);
	SDL_SetWindowResizable(_mainWindow, true);


	if (_mainWindow == NULL)
	{

		SDL_Log("Couldn't create window: %s", SDL_GetError());
		SDL_Quit();
		return;

	}

}


// =================================================


void nothing::Engine::Run()
{

	nothing::LogInfo("Running...");


	SDL_Event event;


	while (isRunning)
	{

		// Poll for pending events
		while (SDL_PollEvent(&event))
		{

			if (event.type == SDL_EVENT_QUIT)
			{

				isRunning = false;

			}

		}

	}

}


// =================================================


void nothing::Engine::Shutdown()
{

	nothing::LogInfo("Shutting down...");


	SDL_DestroyWindow(_mainWindow);
	SDL_Quit();

}


// =================================================