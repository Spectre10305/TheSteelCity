#pragma once
#include <SDL3/SDL.h>


#include "WindowManager.h"
#include "../graphics/RenderManager.h"
#include "../user_interface/UserInterface.h"


namespace nothing
{

	// SCOPO: Gestione principale engine
	class Engine
	{

	public:

		bool Init();
		void Run();
		void Shutdown();


		void HandleEvents(SDL_Event& event);


		bool isRunning = true;

	private:

		WindowManager windowManager_;
		RenderManager renderManager_;
		UserInterface userInterface_;

	};

}