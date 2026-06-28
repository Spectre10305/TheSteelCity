#include "UserInterface.h"
#include "../graphics/IMGUI/imgui.h"
#include "../graphics/IMGUI/imgui_impl_sdl3.h"
#include "../graphics/IMGUI/imgui_impl_opengl3.h"


//=============================================================


void nothing::UserInterface::Init(SDL_Window* windowPtr, void* glContext)
{

	// INIT IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL3_InitForOpenGL(windowPtr, glContext);
	ImGui_ImplOpenGL3_Init();


	// Stile grigio di default
	ImGuiStyle& s = ImGui::GetStyle();
	s.FontSizeBase = 15.0f;


	s.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
	s.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
	s.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);


	s.Colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
	s.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.537f, 0.537f, 0.537f, 0.800);
	s.Colors[ImGuiCol_ButtonActive] = ImVec4(0.567f, 0.567f, 0.567f, 1.0);


	s.Colors[ImGuiCol_Header] = ImVec4(0.572f, 0.572f, 0.572f, 0.310f);
	s.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.537f, 0.537f, 0.537f, 0.800);
	s.Colors[ImGuiCol_HeaderActive] = ImVec4(0.567f, 0.567f, 0.567f, 1.0);


	s.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
	s.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.537f, 0.537f, 0.537f, 0.800);
	s.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.567f, 0.567f, 0.567f, 1.0);


	s.Colors[ImGuiCol_FrameBg] = ImVec4(0.320f, 0.320f, 0.320f, 0.540f);
	s.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.458f, 0.458f, 0.458f, 0.786f);
	s.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.692f, 0.692f, 0.692f, 0.682f);


	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	io.IniFilename = "";

}


//=============================================================


void nothing::UserInterface::Update()
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();


	//ImGui::ShowDemoWindow();


	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("Testing");
	

	if (ImGui::Button("Test Event Fire"))
	{

		currentEvent = UIEvent::TestEvent;

	}


	ImGui::End();


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


void nothing::UserInterface::ClearEvent()
{

	currentEvent = UIEvent::None;

}


//=============================================================