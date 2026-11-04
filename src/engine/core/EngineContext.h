#pragma once


namespace nothing
{

	// Forward declaration per evitare dipendenze circolari
	class WindowManager;
	class AudioManager;
	class ResourceManager;
	class SceneManager;
	class PhysicsManager;
	class InputManager;
	class RenderManager;
	class UserInterface;
	class Filesystem;
	class LocalizationManager;


	// SCOPO: Provvede l'accesso ai sistemi dell'engine sottoforma di riferimenti,
	// questo viene passato ai vari sistemi invece di ogni singolo sistema individuale
	struct EngineContext
	{

		WindowManager*       windowManager    = nullptr;
		AudioManager*        audioManager     = nullptr;
		ResourceManager*     resourcesManager = nullptr;
		SceneManager*        sceneManager     = nullptr;
		PhysicsManager*      physicsManager   = nullptr;
		InputManager*        inputManager     = nullptr;
		RenderManager*       renderManager    = nullptr;
		UserInterface*       userInterface    = nullptr;
		Filesystem*          filesystem       = nullptr;
		LocalizationManager* localManager     = nullptr;


		bool isGamePaused = false;

	};

}