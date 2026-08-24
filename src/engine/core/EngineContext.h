#pragma once


namespace nothing
{

	// Forward declaration per evitare dipendenze circolari
	class WindowManager;
	class ResourceManager;
	class SceneManager;
	class InputManager;
	class RenderManager;
	class UserInterface;
	class Filesystem;


	// SCOPO: Provvede l'accesso ai sistemi dell'engine sottoforma di riferimenti,
	// questo viene passato ai vari sistemi invece di ogni singolo sistema individuale
	struct EngineContext
	{

		WindowManager*   windowManager    = nullptr;
		ResourceManager* resourcesManager = nullptr;
		SceneManager*    sceneManager     = nullptr;
		InputManager*    inputManager     = nullptr;
		RenderManager*   renderManager    = nullptr;
		UserInterface*   userInterface    = nullptr;
		Filesystem*      filesystem       = nullptr;


		bool isGamePaused = false;

	};

}