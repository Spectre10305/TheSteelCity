#include "UserInterface.h"
#include "../graphics/IMGUI/imgui.h"
#include "../graphics/IMGUI/imgui_impl_sdl3.h"
#include "../graphics/IMGUI/imgui_impl_opengl3.h"
#include "../utils/Log.h"
#include "UIStyleHelper.h"
#include <glad/glad.h>
#include <stb_image.h>
#include "../core/WindowManager.h"


//=============================================================


void nothing::UserInterface::Init(EngineContext& ctx)
{
	
	ctx_ = &ctx;
	

	// INIT IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL3_InitForOpenGL(ctx_->windowManager->GetWindowPtr(), ctx_->windowManager->GetGLContext());
	ImGui_ImplOpenGL3_Init();


#pragma region STILE_DEFAULT


	// Stile grigio di default
	ImGuiStyle& s = ImGui::GetStyle();
	s.FontScaleMain = fontScaleBase;


	s.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
	s.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
	s.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);


	s.Colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
	s.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.537f, 0.537f, 0.537f, 0.800f);
	s.Colors[ImGuiCol_ButtonActive] = ImVec4(0.567f, 0.567f, 0.567f, 1.0f);


	s.Colors[ImGuiCol_Header] = ImVec4(0.572f, 0.572f, 0.572f, 0.310f);
	s.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.537f, 0.537f, 0.537f, 0.800f);
	s.Colors[ImGuiCol_HeaderActive] = ImVec4(0.567f, 0.567f, 0.567f, 1.0f);


	s.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
	s.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.537f, 0.537f, 0.537f, 0.800f);
	s.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.567f, 0.567f, 0.567f, 1.0f);


	s.Colors[ImGuiCol_FrameBg] = ImVec4(0.320f, 0.320f, 0.320f, 0.540f);
	s.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.458f, 0.458f, 0.458f, 0.786f);
	s.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.692f, 0.692f, 0.692f, 0.682f);


	s.Colors[ImGuiCol_CheckMark] = ImVec4(0.537f, 0.537f, 0.537f, 0.800f);
	s.Colors[ImGuiCol_SliderGrab] = ImVec4(0.537f, 0.537f, 0.537f, 0.800f);
	s.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.567f, 0.567f, 0.567f, 1.0f);


	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("D:\\TheSteelCity\\assets\\engine\\fonts\\SourceCodePro-Regular.ttf", 20.0f);
	io.IniFilename = ""; // Togliamo il file .INI


	// Imposta i flags che servono per le finestre principali del menu e HUD di gioco, in modo da non
	// avere bordi, titoli, scrollbars, ecc.
	flags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoBringToFrontOnFocus;


#pragma endregion


	currentStyle = NOTHING_WINSTYLE_TECH;


	// TEST TEXTURE
	uiTexture_Logo = CreateUITexture("D:\\TheSteelCity\\assets\\game\\textures\\tex_ui_logo.png");
	uiTexture_Health = CreateUITexture("D:\\TheSteelCity\\assets\\game\\textures\\tex_ui_health_symbol.png");


	currentContext = UIContext::MainMenu;

}


//=============================================================


void nothing::UserInterface::Update()
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();


	switch (currentContext)
	{

	case UIContext::MainMenu:
		UpdateMainMenuContext();
		break;


	case UIContext::Gameplay:
		UpdateGameContext();
		break;


	default:
		break;

	}


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}


//=============================================================


void nothing::UserInterface::Shutdown()
{

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

}


//=============================================================


void nothing::UserInterface::UpdateMainMenuContext()
{

	ImVec2 scrSz = ImGui::GetIO().DisplaySize;


	ImGuiStyle& style = ImGui::GetStyle();
	style.FontScaleMain = fontScaleBase;


	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));


	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);


	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(scrSz.x, scrSz.y));
	ImGui::Begin("MainMenuPanel", nullptr, flags);



	ImGui::Image(uiTexture_Logo, ImVec2(800, 600));


	buttonSpacingY = 0.0f;
	if (MenuButton(">_NUOVA_PARTITA",  ImVec2(buttonOffsetX, (scrSz.y / 2.0f) + buttonSpacingY), ImVec2(buttonWidth, buttonHeight), toleranceY)) { showNewGamePanel = true;  }
	buttonSpacingY += buttonHeight + buttonSpacingOffset;
	if (MenuButton(">_CARICA_PARTITA", ImVec2(buttonOffsetX, (scrSz.y / 2.0f) + buttonSpacingY), ImVec2(buttonWidth, buttonHeight), toleranceY)) { showLoadGamePanel = true; }
	buttonSpacingY += buttonHeight + buttonSpacingOffset;
	if (MenuButton(">_OPZIONI",        ImVec2(buttonOffsetX, (scrSz.y / 2.0f) + buttonSpacingY), ImVec2(buttonWidth, buttonHeight), toleranceY)) { showOptionsPanel = true;  }
	buttonSpacingY += buttonHeight + buttonSpacingOffset;
	MenuButton(">_CONTENUTI_EXTRA",    ImVec2(buttonOffsetX, (scrSz.y / 2.0f) + buttonSpacingY), ImVec2(buttonWidth, buttonHeight), toleranceY);
	buttonSpacingY += buttonHeight + buttonSpacingOffset;
	if (MenuButton(">_ESCI_DAL_GIOCO", ImVec2(buttonOffsetX, (scrSz.y / 2.0f) + buttonSpacingY), ImVec2(buttonWidth, buttonHeight), toleranceY)) { currentEvent = UIEvent::CloseGame; }



	ImGui::End();
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor();


	ImGui::SetNextWindowSize(ImVec2(320, 260), ImGuiCond_FirstUseEver);
	ImGui::Begin("DebugPanel");


	ImGui::PushItemWidth(90.0f);
	ImGui::DragFloat("Button Width",          &buttonWidth, 1.0f, 1.0f, 600.0f);
	ImGui::DragFloat("Button Height",         &buttonHeight, 1.0f, 1.0f, 600.0f);
	ImGui::DragFloat("Button Offset X",       &buttonOffsetX, 1.0f, 1.0f, 600.0f);
	ImGui::DragFloat("Button Spacing offset", &buttonSpacingOffset, 1.0f, 1.0f, 600.0f);
	ImGui::DragInt  ("Button Tolerance Y",    &toleranceY, 1, 0, 600);
	ImGui::DragFloat("Font Scale Base",       &fontScaleBase, 0.1f, 0.1f, 40.0f);
	ImGui::PopItemWidth();


	if (ImGui::Button("Style TECH"))       { currentStyle = NOTHING_WINSTYLE_TECH;       }
	if (ImGui::Button("Style INDUSTRIAL")) { currentStyle = NOTHING_WINSTYLE_INDUSTRIAL; }
	if (ImGui::Button("Style EVIL"))       { currentStyle = NOTHING_WINSTYLE_EVIL;       }


	ImGui::End();


	if (showNewGamePanel)
	{

		nothing::UIStyleHelper::AutoPushStyle(currentStyle);
		static ImVec2 newGamePanelPos = ImVec2(0.0f, 0.0f);
		newGamePanelPos.x = (scrSz.x / 2.0f) - (NEW_GAME_PANEL_WIDTH / 2.0f);
		newGamePanelPos.y = (scrSz.y / 2.0f) - (NEW_GAME_PANEL_HEIGHT / 2.0f);
		ImGui::SetNextWindowPos(newGamePanelPos, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(NEW_GAME_PANEL_WIDTH, NEW_GAME_PANEL_HEIGHT), ImGuiCond_FirstUseEver);
		ImGui::Begin("Nuova Partita", &showNewGamePanel, ImGuiWindowFlags_NoCollapse);


		CenteredText("Capitolo 1");


		ImGui::Separator();
		CenteredText("Atto I - Lo Ierofante");
		ImGui::Separator();
		ImGui::Button("Scena 1 - L'edificio fantasma");
		ImGui::Button("Scena 2 - La Città d'Acciaio");
		ImGui::Button("Scena 3 - Attività illecita");
		ImGui::Separator();
		CenteredText("Atto II - Innaugurazione dell'Eroe");
		ImGui::Separator();
		ImGui::Button("Scena 1 - In buona compagnia");
		ImGui::Button("Scena 2 - La fabbrica");
		ImGui::Button("Scena 3 - La prigione");
		ImGui::Separator();
		CenteredText("Atto III - L'ascesa dell'Impero");
		ImGui::Separator();
		ImGui::Button("Scena 1 - Nel cuore della cittadella");
		ImGui::Button("Scena 2 - L'altro mondo");
		ImGui::Button("Scena 3 - Silenzio radio");


		ImGui::Separator();


		if (ImGui::Button("Chiudi")) { showNewGamePanel = false; }
		ImGui::End();
		nothing::UIStyleHelper::PopStyle();

	}


	if (showLoadGamePanel)
	{

		nothing::UIStyleHelper::AutoPushStyle(currentStyle);
		static ImVec2 loadGamePanelPos = ImVec2(0.0f, 0.0f);
		loadGamePanelPos.x = (scrSz.x / 2.0f) - (LOAD_GAME_PANEL_WIDTH / 2.0f);
		loadGamePanelPos.y = (scrSz.y / 2.0f) - (LOAD_GAME_PANEL_HEIGHT / 2.0f);
		ImGui::SetNextWindowPos(loadGamePanelPos, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(LOAD_GAME_PANEL_WIDTH, LOAD_GAME_PANEL_HEIGHT), ImGuiCond_FirstUseEver);
		ImGui::Begin("Carica Partita", &showLoadGamePanel, ImGuiWindowFlags_NoCollapse);


		ImGui::Button("Slot 1");
		ImGui::SameLine();
		ImGui::Text("<VUOTO>");
		ImGui::Button("Slot 2");
		ImGui::SameLine();
		ImGui::Text("<VUOTO>");
		ImGui::Button("Slot 3");
		ImGui::SameLine();
		ImGui::Text("<VUOTO>");
		ImGui::Button("Slot 4");
		ImGui::SameLine();
		ImGui::Text("<VUOTO>");


		ImGui::End();
		nothing::UIStyleHelper::PopStyle();

	}


	if (showOptionsPanel)
	{

		nothing::UIStyleHelper::AutoPushStyle(currentStyle);
		static ImVec2 optionsPanelPos = ImVec2(0.0f, 0.0f);
		optionsPanelPos.x = (scrSz.x / 2.0f) - (OPTIONS_PANEL_WIDTH / 2.0f);
		optionsPanelPos.y = (scrSz.y / 2.0f) - (OPTIONS_PANEL_HEIGHT / 2.0f);
		ImGui::SetNextWindowPos(optionsPanelPos, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(OPTIONS_PANEL_WIDTH, OPTIONS_PANEL_HEIGHT), ImGuiCond_FirstUseEver);
		ImGui::Begin("Opzioni", &showOptionsPanel, ImGuiWindowFlags_NoCollapse);


		// SEZIONE DI GIOCO
		CenteredText("Gioco");
		ImGui::Separator();


		static const char* difficultyItems[] = { "Facile", "Normale", "Difficile" };
		static int currentDifficultyItem = 1;
		ImGui::Combo("Difficoltà", &currentDifficultyItem, difficultyItems, IM_ARRAYSIZE(difficultyItems));


		static bool fastAnimations = false;
		ImGui::Checkbox("Animazioni veloci (?)", &fastAnimations);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
		{

			ImGui::SetTooltip("Velocizza o rimuove alcune\nanimazioni di gioco o dell'HUD");

		}


		static bool autoRun = false;
		ImGui::Checkbox("Corsa automatica", &autoRun);


		static const char* languageItems[] = { "Italiano", "English", "Français", "Español" };
		static int currentLanguageItem = 0;
		ImGui::Combo("Lingua", &currentLanguageItem, languageItems, IM_ARRAYSIZE(languageItems));


		// SEZIONE DI GRAFICA
		CenteredText("Grafica - Schermo");
		ImGui::Separator();


		static const char* resolutionItems[] = { "800x600", "1024x768", "1280x720", "1920x1080" };
		static int currentResolutionItem = 3;
		ImGui::Combo("Risoluzione", &currentResolutionItem, resolutionItems, IM_ARRAYSIZE(resolutionItems));


		static const char* fullscreenItems[] = { "Finestra", "Schermo intero", "Senza bordi" };
		static int currentFullscreenItem = 0;
		ImGui::Combo("Modalità schermo", &currentFullscreenItem, fullscreenItems, IM_ARRAYSIZE(fullscreenItems));


		static int brightness = 50;
		ImGui::SliderInt("Luminosità", &brightness, 0, 100);


		static int contrast = 40;
		ImGui::SliderInt("Contrasto", &contrast, 0, 100);


		static bool vsync = true;
		ImGui::Checkbox("Usa V-Sync", &vsync);


		static int textureQuality = 5;
		static int models3DQuality = 5;
		ImGui::SliderInt("Qualità texture", &textureQuality, 1, 5);
		ImGui::SliderInt("Qualità modelli 3D", &models3DQuality, 1, 5);


		// SEZIONE AUDIO
		CenteredText("Audio");
		ImGui::Separator();


		static int masterVolume = 100;
		static int musicVolume = 80;
		static int sfxVolume = 85;
		ImGui::SliderInt("Volume master", &masterVolume, 0, 100);
		ImGui::SliderInt("Volume musica", &musicVolume, 0, 100);
		ImGui::SliderInt("Volume effetti", &sfxVolume, 0, 100);


		static bool muteInBackground = false;
		ImGui::Checkbox("Disattiva audio in background (?)", &muteInBackground);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
		{

			ImGui::SetTooltip("Disattiva l'audio di gioco quando\nun'altra finestra è in focus");

		}


		static bool useSteamAudio = true;
		ImGui::Checkbox("Usa API Steam Audio (?)", &useSteamAudio);
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
		{

			ImGui::SetTooltip("Usa l'API di Steam Audio per\neffetti audio più realistici");

		}


		// SEZIONE CONTROLLI
		CenteredText("Controlli mouse - tastiera");
		ImGui::Separator();


		ImGui::Button("W", ImVec2(200, 0));
		ImGui::SameLine();
		ImGui::Text("Cammina avanti");


		ImGui::Button("Mouse 1", ImVec2(200, 0));
		ImGui::SameLine();
		ImGui::Text("Mira");


		ImGui::Button("Mouse 0", ImVec2(200, 0));
		ImGui::SameLine();
		ImGui::Text("Spara");


		ImGui::Button("Mouse scroll SU'", ImVec2(200, 0));
		ImGui::SameLine();
		ImGui::Text("Arma precedente");


		ImGui::Button("Mouse scroll GIU'", ImVec2(200, 0));
		ImGui::SameLine();
		ImGui::Text("Arma successiva");


		ImGui::Button("E", ImVec2(200, 0));
		ImGui::SameLine();
		ImGui::Text("Usa");


		ImGui::Button("Spazio", ImVec2(200, 0));
		ImGui::SameLine();
		ImGui::Text("Movimento speciale");


		ImGui::Button("\\", ImVec2(200, 0));
		ImGui::SameLine();
		ImGui::Text("Apri console di sviluppo");


		// SEZIONE CONTROLLI
		CenteredText("Altro");
		ImGui::Separator();


		static bool useDevConsole = false;
		ImGui::Checkbox("Usa console di sviluppo", &useDevConsole);


		ImGui::Button("Apri cartella di gioco", ImVec2(300, 0));
		ImGui::SameLine();
		ImGui::Button("Apri cartella di salvataggi", ImVec2(340, 0));


		ImGui::Spacing();
		ImGui::Spacing();


		ImGui::Text("Stile dell'interfaccia utente");


		if (ImGui::Button("Stile Tech",        ImVec2(300.0f, 0.0f))) { currentStyle = NOTHING_WINSTYLE_TECH; }
		if (ImGui::Button("Stile Industriale", ImVec2(300.0f, 0.0f))) { currentStyle = NOTHING_WINSTYLE_INDUSTRIAL; }
		if (ImGui::Button("Stile Cattivo",     ImVec2(300.0f, 0.0f))) { currentStyle = NOTHING_WINSTYLE_EVIL; }


		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		if (ImGui::Button("Applica", ImVec2(110, 0))) { currentEvent = UIEvent::ApplyOptions; }
		ImGui::SameLine();
		if (ImGui::Button("Ripristina", ImVec2(110, 0))) { currentEvent = UIEvent::ResetOptions; }


		ImGui::End();
		nothing::UIStyleHelper::PopStyle();

	}

}


//=============================================================


void nothing::UserInterface::UpdateGameContext()
{

	ImVec2 scrSz = ImGui::GetIO().DisplaySize;


	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));


	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);


	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(scrSz.x, scrSz.y));
	ImGui::Begin("GameHUDPanel", nullptr, flags);


	ImDrawList* dl = ImGui::GetForegroundDrawList();


	// Letterbox
	static float letterboxHeight = 90.0f;
	dl->AddRectFilled(ImVec2(0, 0), ImVec2(scrSz.x, letterboxHeight), IM_COL32(0, 0, 0, 255));
	dl->AddRectFilled(ImVec2(0, scrSz.y), ImVec2(scrSz.x, scrSz.y - letterboxHeight), IM_COL32(0, 0, 0, 255));


	// Simbolo vita ("+")
	dl->AddImage(uiTexture_Health, ImVec2(10, scrSz.y - 10), ImVec2(letterboxHeight - 10, (scrSz.y - letterboxHeight) + 10));


	static float x = 60;
	static float y = 60;
	static float width = 520;
	static float width2 = 520;
	static float height = 10;
	dl->AddRectFilled(ImVec2(x, scrSz.y - letterboxHeight + y), ImVec2(width, scrSz.y - height), IM_COL32(90, 90, 90, 255));


	ImU32 barColor = IM_COL32(0, 250, 250, 255);


	if (width2 < 325.0f)
	{

		barColor = IM_COL32(255, 216, 0, 255);

	}


	if (width2 < 200.0f)
	{

		barColor = IM_COL32(255, 0, 0, 255);

	}


	dl->AddRectFilled(ImVec2(x, scrSz.y - letterboxHeight + y), ImVec2(width2, scrSz.y - height), barColor);


	dl->AddText(ImVec2(20, 20), IM_COL32(255, 255, 255, 255), "HUD di gioco in sviluppo...");


	ImGui::End();
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor();


	ImGui::Begin("sasd");
	ImGui::SliderFloat("X", &x, 0.0f, scrSz.x);
	ImGui::SliderFloat("Y", &y, 0.0f, scrSz.y);
	ImGui::SliderFloat("Width", &width, 0.0f, scrSz.x);
	ImGui::SliderFloat("Width2", &width2, 125.0f, 520.0f);
	ImGui::SliderFloat("Height", &height, 0.0f, scrSz.y);
	ImGui::End();

}


//=============================================================


void nothing::UserInterface::ClearEvent()
{

	currentEvent = UIEvent::None;

}


//=============================================================


bool nothing::UserInterface::MenuButton(const char* text, ImVec2 pos, ImVec2 size, int textYTolerance)
{

	ImDrawList* draw = ImGui::GetForegroundDrawList();


	ImGui::SetCursorScreenPos(pos);
	ImGui::InvisibleButton(text, size);


	bool hovered = ImGui::IsItemHovered();
	bool clicked = ImGui::IsItemClicked();


	ImU32 col = 0;


	switch (currentStyle)
	{

		case NOTHING_WINSTYLE_TECH:
			col = hovered ? IM_COL32(0, 245, 245, 240) : IM_COL32(0, 245, 245, 120);
			break;


		case NOTHING_WINSTYLE_INDUSTRIAL:
			col = hovered ? IM_COL32(240, 240, 240, 240) : IM_COL32(240, 240, 240, 120);
			break;


		case NOTHING_WINSTYLE_EVIL:
			col = hovered ? IM_COL32(245, 0, 0, 240) : IM_COL32(245, 0, 0, 120);
			break;


		default:
			col = hovered ? IM_COL32(0, 245, 245, 240) : IM_COL32(0, 245, 245, 120);
			break;

	}	


	// Rettangolo del bottone
	draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), col);


	// Testo del bottone
	draw->AddText(ImVec2(pos.x + 20, pos.y + size.y - (size.y / 2) - textYTolerance), IM_COL32(255, 255, 255, 255), text);


	if (clicked)
	{

		return true;

	}


	return false;

}


//=============================================================


void nothing::UserInterface::CenteredText(const char* text)
{

	auto windowWidth = ImGui::GetWindowSize().x;
	auto textWidth = ImGui::CalcTextSize(text).x;


	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text(text);

}


//=============================================================


uint32_t nothing::UserInterface::CreateUITexture(const char* texturePath)
{

	uint32_t res;


	glGenTextures(1, &res);
	glBindTexture(GL_TEXTURE_2D, res);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(false);


	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);


	if (data)
	{

		switch (nrChannels)
		{

		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;


		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;


		default:
			nothing::LogError("Unsupported number of channels in UI texture");
			break;

		}


		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else
	{

		nothing::LogError("Failed to load UI texture");

	}


	stbi_image_free(data);


	return res;

}


//=============================================================