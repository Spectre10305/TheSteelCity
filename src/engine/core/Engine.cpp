#include "Engine.h"
#include "../utils/Log.h"
#include "../utils/Time.h"
#include "../graphics/IMGUI/imgui_impl_sdl3.h" // ImGui_ImplSDL3_ProcessEvent() in HandleEvents()


// =================================================


bool nothing::Engine::Init()
{

	nothing::LogInfo("Initializing engine...");


	nothing::StartTimer();


	engineContext_.windowManager    = &windowManager_;
	engineContext_.resourcesManager = &resourceManager_;
	engineContext_.sceneManager     = &sceneManager_;
	engineContext_.inputManager     = &inputManager_;
	engineContext_.renderManager    = &renderManager_;
	engineContext_.userInterface    = &userInterface_;


	if (!windowManager_.Init())
	{

		nothing::LogInfo("Can't initialize WindowManager, shutting down everything...");
		return false;

	}
	

	resourceManager_.InitDefaults();


	sceneManager_.Init(engineContext_);


	renderManager_.Init(engineContext_);
	renderManager_.SetBackgroundColor(0.5f, 0.5f, 0.5f);


	int wWidth = 0;
	int wHeight = 0;
	windowManager_.GetWindowSize(wWidth, wHeight);
	renderManager_.SetAspectRatio(wWidth, wHeight);


	userInterface_.Init(engineContext_);

	
	inputManager_.BindKey(SDL_SCANCODE_W,      GameAction::MoveForward);
	inputManager_.BindKey(SDL_SCANCODE_A,      GameAction::MoveLeft);
	inputManager_.BindKey(SDL_SCANCODE_S,      GameAction::MoveBackward);
	inputManager_.BindKey(SDL_SCANCODE_D,      GameAction::MoveRight);
	inputManager_.BindKey(SDL_SCANCODE_LEFT,   GameAction::RotateLeft);
	inputManager_.BindKey(SDL_SCANCODE_RIGHT,  GameAction::RotateRight);
	inputManager_.BindKey(SDL_SCANCODE_UP,     GameAction::RotateUp);
	inputManager_.BindKey(SDL_SCANCODE_DOWN,   GameAction::RotateDown);
	inputManager_.BindKey(SDL_SCANCODE_GRAVE,  GameAction::OpenDevConsole);
	inputManager_.BindKey(SDL_SCANCODE_ESCAPE, GameAction::Exit);


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


		switch (gameState_)
		{

		case GameState::MainMenu:
			userInterface_.Update();
			break;


		case GameState::Gameplay:
			sceneManager_.Update();
			renderManager_.Update(deltaTime);
			userInterface_.Update();
			break;


		default:
			break;

		}


		if (inputManager_.IsActionTriggered(GameAction::OpenDevConsole))
		{

			nothing::LogInfo("Open Dev Console");

		}


		windowManager_.SwapBuffers();


		// Cancelliamo lo stato dei tasti premuti per il frame successivo
		inputManager_.ClearInput();


		//static double fps = 0.0;
		//fps = 1.0 / deltaTime;

	}

}


// =================================================


void nothing::Engine::Shutdown()
{

	nothing::LogInfo("Shutting down...");


	if (gameState_ == GameState::Gameplay)
	{

		nothing::LogInfo("Quitting from game state, cleaning up resources...");
		sceneManager_.UnloadScene();
		resourceManager_.DeleteAllTextures();

	}


	renderManager_.Shutdown();
	sceneManager_.Shutdown();
	windowManager_.Shutdown();

}


// =================================================


void nothing::Engine::HandleEvents(SDL_Event& event)
{

	// Eventi SDL (finestra, tastiera, mouse)
	while (SDL_PollEvent(&event))
	{

		// Eventi per IMGUI
		ImGui_ImplSDL3_ProcessEvent(&event);


		inputManager_.ProcessEvent(event);


		switch (event.type)
		{

		case SDL_EVENT_WINDOW_RESIZED:
			int w, h;
			windowManager_.   GetWindowSize(w, h);
			renderManager_.ResizeGLViewport(w, h);
			renderManager_.  SetAspectRatio(w, h);
			nothing::LogInfo("Window resized to: " + std::to_string(w) + " X " + std::to_string(h));
			break;


		case SDL_EVENT_QUIT:
			isRunning = false;
			break;
			

		default:
			break;

		}

	}


	// -------------------------------------------------------------------------------------------------------------


	// Eventi UI
	switch (userInterface_.currentEvent)
	{

	case UIEvent::BeginGame:
		sceneManager_.LoadScene();
		userInterface_.SwitchContext(UIContext::Gameplay);
		gameState_ = GameState::Gameplay;
		break;


	case UIEvent::ReturnToMenu:
		sceneManager_.UnloadScene();
		resourceManager_.DeleteAllTextures();
		userInterface_.SwitchContext(UIContext::MainMenu);
		gameState_ = GameState::MainMenu;
		break;


	case UIEvent::ApplyOptions:
		break;


	case UIEvent::ResetOptions:
		break;


	case UIEvent::CloseGame:
		isRunning = false;
		break;


	default:
		break;

	}


	// Cancella gli eventi UI
	userInterface_.ClearEvent();


}


// =================================================