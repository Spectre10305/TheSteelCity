#pragma once
#include <SDL3/SDL.h>


namespace nothing
{

	enum class UIEvent
	{

		None,
		TestEvent

	};


	// SCOPO: Gestione principale dell'interfaccia utente,
	// powered by IMGUI
	class UserInterface
	{

	public:

		void Init(SDL_Window* windowPtr, void* glContext);
		void Update();
		void Shutdown();


		void ClearEvent();


		UIEvent currentEvent = UIEvent::None;

	};

}