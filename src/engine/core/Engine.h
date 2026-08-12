#pragma once
#include <SDL3/SDL.h>


#include "WindowManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Filesystem.h"
#include "../graphics/RenderManager.h"
#include "../user_interface/UserInterface.h"


#include "EngineContext.h"


namespace nothing
{

	enum class GameState
	{

		MainMenu,
		Gameplay

	};



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


		Filesystem      fileSystem_;
		WindowManager   windowManager_;
		RenderManager   renderManager_;
		UserInterface   userInterface_;
		InputManager    inputManager_;
		ResourceManager resourceManager_;
		SceneManager    sceneManager_;


		GameState gameState_ = GameState::MainMenu;

	};

}