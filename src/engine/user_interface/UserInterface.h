#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>
#include "../graphics/IMGUI/imgui.h"


#define NEW_GAME_PANEL_WIDTH   500.0f
#define NEW_GAME_PANEL_HEIGHT  350.0f
#define LOAD_GAME_PANEL_WIDTH  500.0f
#define LOAD_GAME_PANEL_HEIGHT 350.0f
#define OPTIONS_PANEL_WIDTH    800.0f
#define OPTIONS_PANEL_HEIGHT   450.0f


namespace nothing
{

	enum class UIEvent
	{

		None,
		BeginGame,
		CloseGame,
		ApplyOptions,
		ResetOptions

	};


	enum class UIContext
	{

		None,
		MainMenu,
		Gameplay

	};


	// SCOPO: Gestione principale dell'interfaccia utente,
	// powered by IMGUI
	class UserInterface
	{

	public:

		void Init(SDL_Window* windowPtr, void* glContext);
		void Update();
		void Shutdown();


		ImGuiWindowFlags flags;


		UIContext currentContext = UIContext::None;


		uint32_t currentStyle = 0;


		void     UpdateMainMenuContext();
		void     UpdateGameContext();
		void     ClearEvent();
		bool     MenuButton(const char* text, ImVec2 pos, ImVec2 size, int textYTolerance);
		void     CenteredText(const char* text); // Helper per testo centrato
		uint32_t CreateUITexture(const char* texturePath);


		float buttonWidth         = 250.0f;
		float buttonHeight        = 40.0f;
		float buttonOffsetX       = 20.0f;
		float buttonSpacingY      = 0.0f;
		float buttonSpacingOffset = 10.0f;
		float fontScaleBase       = 1.0f;
		int   toleranceY          = 7;


		bool showNewGamePanel  = false;
		bool showLoadGamePanel = false;
		bool showOptionsPanel  = false;


		// Texture UI
		uint32_t uiTexture_Logo = 0;
		uint32_t uiTexture_Health = 0;


		UIEvent currentEvent = UIEvent::None;

	};

}