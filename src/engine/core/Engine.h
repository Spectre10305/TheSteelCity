#pragma once
#include <SDL3/SDL.h>


#include "WindowManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "../graphics/RenderManager.h"
#include "../user_interface/UserInterface.h"


#include "EngineContext.h"


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

		EngineContext   engineContext_{};


		WindowManager   windowManager_;
		RenderManager   renderManager_;
		UserInterface   userInterface_;
		InputManager    inputManager_;
		ResourceManager resourceManager_;
		SceneManager    sceneManager_;


	};

}