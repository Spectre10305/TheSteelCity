#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>


// La coppia che scoppia
#include <optional>
#include <tweeny/tween.h>


#include "../graphics/IMGUI/imgui.h"
#include "../core/EngineContext.h"


#define NEW_GAME_PANEL_WIDTH        550.0f
#define NEW_GAME_PANEL_HEIGHT       350.0f
#define LOAD_GAME_PANEL_WIDTH       500.0f
#define LOAD_GAME_PANEL_HEIGHT      350.0f
#define OPTIONS_PANEL_WIDTH         800.0f
#define OPTIONS_PANEL_HEIGHT        450.0f
#define BACK_TO_MENU_WARNING_WIDTH  590.0f
#define BACK_TO_MENU_WARNING_HEIGHT 160.0f


namespace nothing
{

	enum class UIEvent
	{

		None,
		BeginGame,
		SwitchToGameContext,
		ReturnToMenu,
		CloseGame,
		ApplyOptions,
		ResetOptions

	};


	enum class UIContext
	{

		None,
		MainMenu,
		Gameplay,
		SplashScreen,
		LoadingScreen

	};


	// SCOPO: Gestione principale dell'interfaccia utente,
	// powered by IMGUI
	class UserInterface
	{

	public:

		void Init(EngineContext& ctx);
		void Update(double deltaTime);
		void Shutdown();


		ImGuiWindowFlags flags;


		UIContext currentContext = UIContext::None;


		uint32_t currentStyle = 0;


		// Qesto diventa VERO per evitare che LoadingScreenContext
		// dia sempre un evento BeginGame al fade in dello schermo nero
		bool isGameLoaded = false;


		void     UpdateMainMenuContext(double deltaTime);
		void     UpdateGameContext();
		void     UpdateSplashScreenContext();
		void     UpdateLoadingScreenContext();
		void     UpdateLoadingScreenBarValue(int val); // Valore in percentuale 0 - 100%
		void     SetIsGameLoaded(bool val); // Questo imposta isGameLoaded
		void     SwitchContext(UIContext newContext);
		void     ClearEvent();
		bool     MenuButton(const char* text, ImVec2 pos, ImVec2 size, int textYTolerance);
		void     CenteredText(const char* text); // Helper per testo centrato
		uint32_t CreateUITexture(const char* texturePath, bool flipVert, int32_t filter);


		void ShowNewGamePanel(ImVec2& scrSz);
		void ShowLoadGamePanel(ImVec2& scrSz);
		void ShowOptionsPanel(ImVec2& scrSz);
		void ShowBackToMenuWarning(ImVec2& scrSz);
		void ShowDevConsole(ImVec2& scrSz);


		float buttonWidth         = 250.0f;
		float buttonHeight        = 47.0f;
		float buttonOffsetX       = 20.0f;
		float buttonSpacingY      = 0.0f;
		float buttonSpacingOffset = 10.0f;
		float fontScaleBase       = 1.0f;
		int   toleranceY          = 10;


		bool showNewGamePanel      = false;
		bool showLoadGamePanel     = false;
		bool showOptionsPanel      = false;
		bool showPauseMenu         = false;
		bool showBackToMenuWarning = false;


		bool showDevConsole = false;


		// Texture UI
		uint32_t uiTexture_Logo        = 0;
		uint32_t uiTexture_Health      = 0;
		uint32_t uiTexture_NothingLogo = 0;
		uint32_t uiTexture_MenuAnim1   = 0;


		// Font di default
		ImFont* defaultFont = nullptr;


		UIEvent currentEvent = UIEvent::None;


	private:

		EngineContext* ctx_ = nullptr;


		// Animazioni UI, immagini che scorrono sullo schermo
		bool  doFadePanelOut_     = true;
		bool  doFadePanelIn_      = false;
		bool  canRunUIAnimations_ = false;


		// Questo è FALSO quando il fade panel è in movimento,
		// evita che il giocatore usi i tasti del menù per non
		// causare un softlock quando il pannello non è completamente
		// scuro o trasparente
		bool  canPressMenuButtons_ = false;
		int   fadePanelAlpha_      = 255;
		int   fadeSpeed_           = 3;
		float loadingBarWidth_     = 0.0f;


		bool anim1Active_ = true;
		std::optional<tweeny::tween<float, int>> testUIAnim1_;
		void RunUIAnimations(ImDrawList* _dl, ImVec2& scrSz, double deltaTime);


		// Dev console
		std::vector<std::string> lines_;
		char buf_[32];
		ImFont* consoleFont = nullptr;

	};

}