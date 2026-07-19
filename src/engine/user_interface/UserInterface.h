#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include "../graphics/IMGUI/imgui.h"
#include "../core/EngineContext.h"


#define NEW_GAME_PANEL_WIDTH        500.0f
#define NEW_GAME_PANEL_HEIGHT       350.0f
#define LOAD_GAME_PANEL_WIDTH       500.0f
#define LOAD_GAME_PANEL_HEIGHT      350.0f
#define OPTIONS_PANEL_WIDTH         800.0f
#define OPTIONS_PANEL_HEIGHT        450.0f
#define BACK_TO_MENU_WARNING_WIDTH  500.0f
#define BACK_TO_MENU_WARNING_HEIGHT 120.0f


namespace nothing
{

	enum class UIEvent
	{

		None,
		BeginGame,
		ReturnToMenu,
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

		void Init(EngineContext& ctx);
		void Update();
		void Shutdown();


		ImGuiWindowFlags flags;


		UIContext currentContext = UIContext::None;


		uint32_t currentStyle = 0;


		void     UpdateMainMenuContext();
		void     UpdateGameContext();
		void     SwitchContext(UIContext newContext);
		void     ClearEvent();
		bool     MenuButton(const char* text, ImVec2 pos, ImVec2 size, int textYTolerance);
		void     CenteredText(const char* text); // Helper per testo centrato
		uint32_t CreateUITexture(const char* texturePath);


		void ShowNewGamePanel(ImVec2& scrSz);
		void ShowLoadGamePanel(ImVec2& scrSz);
		void ShowOptionsPanel(ImVec2& scrSz);
		void ShowBackToMenuWarning(ImVec2& scrSz);
		void ShowDevConsole(ImVec2& scrSz);


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


		bool showPauseMenu = false;
		bool showBackToMenuWarning = false;


		bool showDevConsole = false;


		// Texture UI
		uint32_t uiTexture_Logo = 0;
		uint32_t uiTexture_Health = 0;


		UIEvent currentEvent = UIEvent::None;


	private:

		EngineContext* ctx_ = nullptr;


		// Dev console
		std::vector<std::string> lines_;
		char buf_[32];


	};

}