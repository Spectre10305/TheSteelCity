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
		Exit

	};


	// SCOPO: Gestione dell'input da tastiera e mouse (controller in seguito)
	class InputManager
	{

	public:


		void ProcessEvent(const SDL_Event& evt);
		void ClearInput();


		//binding
		void BindKey(SDL_Scancode key, GameAction action);
		void UnBindKey(GameAction action);


		//controllo tasti
		bool IsActionTriggered(GameAction action) const;
		bool IsActionHeld(GameAction action) const;
		bool IsActionReleased(GameAction action) const;

	private:

		const bool* currentKeyStates = nullptr;


		std::unordered_map<SDL_Scancode, GameAction> keyToAction;
		std::unordered_map<GameAction, SDL_Scancode> actionToKey;


		std::set<GameAction> actionsPressed;
		std::set<GameAction> actionsReleased;


	};

}