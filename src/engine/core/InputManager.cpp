#include "InputManager.h"
#include "../utils/Log.h"


//=============================================================


void nothing::InputManager::ProcessEvent(const SDL_Event& evt)
{

	if (evt.type == SDL_EVENT_KEY_DOWN && !evt.key.repeat)
	{

		auto mIt = keyToAction.find(evt.key.scancode);


		if (mIt != keyToAction.end())
		{

			actionsPressed.insert(mIt->second);

		}

	}



	if (evt.type == SDL_EVENT_KEY_UP)
	{

		auto mIt = keyToAction.find(evt.key.scancode);


		if (mIt != keyToAction.end())
		{

			actionsReleased.insert(mIt->second);

		}

	}


	currentKeyStates = SDL_GetKeyboardState(nullptr);


	/*
	if (evt.type == SDL_EVENT_MOUSE_MOTION)
	{

		mouseXDelta = evt.motion.xrel;
		mouseYDelta = evt.motion.yrel;

	}
	*/

}


//=============================================================


void nothing::InputManager::ClearInput()
{

	actionsPressed.clear();
	actionsReleased.clear();

}


//=============================================================


void nothing::InputManager::BindKey(SDL_Scancode key, GameAction action)
{

	keyToAction[key] = action;
	actionToKey[action] = key;

}


//=============================================================


void nothing::InputManager::UnBindKey(GameAction action)
{

	auto mIt = actionToKey.find(action);


	if (mIt != actionToKey.find(action))
	{

		keyToAction.erase(mIt->second);
		actionToKey.erase(mIt);

	}

}


//=============================================================


bool nothing::InputManager::IsActionTriggered(GameAction action) const
{

	return actionsPressed.find(action) != actionsPressed.end();

}


//=============================================================


bool nothing::InputManager::IsActionHeld(GameAction action) const
{

	auto mIt = actionToKey.find(action);


	if (mIt == actionToKey.end())
	{

		return false;

	}


	return currentKeyStates[mIt->second];

}


//=============================================================


bool nothing::InputManager::IsActionReleased(GameAction action) const
{

	return actionsReleased.find(action) != actionsReleased.end();

}


//=============================================================