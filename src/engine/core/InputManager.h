#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>
#include <set>


namespace nothing
{

	enum class GameAction
	{

		MoveForward,
		MoveBackward,
		MoveLeft,
		MoveRight,
		RotateLeft,
		RotateRight,
		RotateUp,
		RotateDown,
		Jump,
		Use,
		Fire,
		Reload,
		CameraFovUP,
		CamerFovDown,
		OpenDevConsole,
		Exit

	};


	// SCOPO: Gestione dell'input da tastiera e mouse (controller in seguito)
	class InputManager
	{

	public:


		void ProcessEvent(const SDL_Event& evt);
		void ClearInput();


		// Binding
		void BindKey(SDL_Scancode key, GameAction action);
		void UnBindKey(GameAction action);


		// Controllo tasti
		bool IsActionTriggered(GameAction action) const;
		bool IsActionHeld(GameAction action) const;
		bool IsActionReleased(GameAction action) const;


		// Controllo mouse
		void GetMouseDelta(float& mx, float& my);

	private:

		const bool* currentKeyStates = nullptr;


		std::unordered_map<SDL_Scancode, GameAction> keyToAction;
		std::unordered_map<GameAction, SDL_Scancode> actionToKey;


		std::set<GameAction> actionsPressed;
		std::set<GameAction> actionsReleased;


		float mouseXDelta = 0.0f;
		float mouseYDelta = 0.0f;


	};

}