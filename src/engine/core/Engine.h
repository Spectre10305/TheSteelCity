#pragma once
#include <SDL3/SDL.h>


namespace nothing
{

	// SCOPO: Gestione principale engine
	class Engine
	{

	public:

		void Init();
		void Run();
		void Shutdown();

		bool isRunning = true;

	private:

		SDL_Window* _mainWindow = nullptr;

	};

}