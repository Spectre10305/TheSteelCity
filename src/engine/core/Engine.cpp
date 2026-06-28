#include "Engine.h"
#include "../utils/Log.h"
#include "../utils/Time.h"
#include "../graphics/IMGUI/imgui.h"
#include "../graphics/IMGUI/imgui_impl_sdl3.h"
#include "../graphics/IMGUI/imgui_impl_opengl3.h"


// =================================================


bool nothing::Engine::Init()
{

	nothing::LogInfo("Initializing engine...");


	nothing::StartTimer();


	if (!windowManager_.Init())
	{

		nothing::LogInfo("Can't initialize WindowManager, shutting down everything...");
		return false;

	}


	renderManager_.Init();
	renderManager_.SetBackgroundColor(0.0f, 0.0f, 0.0f);


	int wWidth = 0;
	int wHeight = 0;
	windowManager_.GetWindowSize(wWidth, wHeight);
	renderManager_.SetAspectRatio(wWidth, wHeight);


	double timeToInitialize = nothing::ElapsedMS();
	nothing::LogInfo("Took: " + std::to_string(timeToInitialize) + " MS to initialize engine");


	// INIT IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL3_InitForOpenGL(windowManager_.GetWindowPtr(), windowManager_.GetGLContext());
	ImGui_ImplOpenGL3_Init();


	ImGuiStyle& s = ImGui::GetStyle();
	s.FontSizeBase = 15.0f;


	s.Colors[ImGuiCol_WindowBg]      = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
	s.Colors[ImGuiCol_TitleBg]       = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
	s.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);


	s.Colors[ImGuiCol_Button]        = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
	s.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.537f, 0.537f, 0.537f, 0.800);
	s.Colors[ImGuiCol_ButtonActive]  = ImVec4(0.567f, 0.567f, 0.567f, 1.0);


	s.Colors[ImGuiCol_Header]        = ImVec4(0.572f, 0.572f, 0.572f, 0.310f);
	s.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.537f, 0.537f, 0.537f, 0.800);
	s.Colors[ImGuiCol_HeaderActive]  = ImVec4(0.567f, 0.567f, 0.567f, 1.0);


	s.Colors[ImGuiCol_ResizeGrip]        = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
	s.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.537f, 0.537f, 0.537f, 0.800);
	s.Colors[ImGuiCol_ResizeGripActive]  = ImVec4(0.567f, 0.567f, 0.567f, 1.0);


	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	io.IniFilename = "";


	return true;

}


// =================================================


void nothing::Engine::Run()
{

	nothing::LogInfo("Running...");


	while (isRunning)
	{

		double deltaTime = nothing::CalcDeltaTime();


		SDL_Event event;
		HandleEvents(event);


		renderManager_.Update();


		SDL_GL_SwapWindow(windowManager_.GetWindowPtr());


		windowManager_.SetWindowTitle(("Nothing Engine DEMO - DeltaTime: " + std::to_string(deltaTime)).c_str());

	}

}


// =================================================


void nothing::Engine::Shutdown()
{

	nothing::LogInfo("Shutting down...");


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();


	renderManager_.Shutdown();
	windowManager_.Shutdown();

}


// =================================================


void nothing::Engine::HandleEvents(SDL_Event& event)
{

	// Eventi SDL (finestra, tastiera, mouse)
	while (SDL_PollEvent(&event))
	{

		// Eventi per IMGUI
		ImGui_ImplSDL3_ProcessEvent(&event);


		switch (event.type)
		{

		case SDL_EVENT_WINDOW_RESIZED:
			int w, h;
			windowManager_.   GetWindowSize(w, h);
			renderManager_.ResizeGLViewport(w, h);
			renderManager_.  SetAspectRatio(w, h);
			nothing::LogInfo("Window resized to: " + std::to_string(w) + "X" + std::to_string(h));
			break;


		case SDL_EVENT_QUIT:
			isRunning = false;
			break;
			

		default:
			break;

		}

	}

}


// =================================================