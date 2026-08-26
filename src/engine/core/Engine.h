#pragma once
#include <SDL3/SDL.h>


#include "WindowManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Filesystem.h"
#include "../graphics/RenderManager.h"
#include "../user_interface/UserInterface.h"
#include "../physics/PhysicsManager.h"


#include "EngineContext.h"


namespace nothing
{

	enum class GameState
	{

		None,
		SplashScreen,
		MainMenu,
		Gameplay

	};


	// SCOPO: Contiene i dati degli argomenti di
	// avvio della riga di comando
	struct LaunchOptions
	{

		bool noSplash = false;

	};


	// SCOPO: Gestione principale engine
	class Engine
	{

	public:

		bool Init(LaunchOptions& lopts);
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
		PhysicsManager  physicsManager_;


		GameState gameState_ = GameState::None;

	};

}