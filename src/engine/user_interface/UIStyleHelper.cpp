#include "UIStyleHelper.h"
#include "../graphics/IMGUI/imgui.h"


// =================================================


void nothing::UIStyleHelper::AutoPushStyle(int style)
{

	switch (style)
	{

	case NOTHING_WINSTYLE_TECH:
		PushTech();
		break;


	case NOTHING_WINSTYLE_INDUSTRIAL:
		PushIndustrial();
		break;


	case NOTHING_WINSTYLE_EVIL:
		PushEvil();
		break;


	default:
		break;

	}

}


// =================================================


void nothing::UIStyleHelper::PushTech()
{

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 250));
	ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 255, 255, 255));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, IM_COL32(43, 85, 86, 255));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(15, 171, 185, 255));


	ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(65, 205, 250, 105));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(65, 235, 250, 165));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(15, 215, 250, 205));


	ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(40, 115, 125, 140));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(65, 210, 250, 105));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(65, 225, 250, 170));


	ImGui::PushStyleColor(ImGuiCol_ResizeGrip, IM_COL32(65, 241, 250, 65));
	ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, IM_COL32(65, 230, 250, 175));
	ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, IM_COL32(65, 205, 250, 245));


	ImGui::PushStyleColor(ImGuiCol_CheckMark, IM_COL32(65, 241, 250, 250));
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(65, 230, 250, 175));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, IM_COL32(65, 205, 250, 245));


	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.5f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.0f);

}


// =================================================


void nothing::UIStyleHelper::PushIndustrial()
{

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(10, 10, 10, 220));
	ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(80, 80, 80, 255));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, IM_COL32(50, 50, 50, 255));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(90, 90, 90, 255));


	ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(50, 50, 50, 200));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(80, 80, 80, 250));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(100, 100, 100, 255));


	ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(100, 100, 100, 130));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(100, 100, 100, 185));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(110, 110, 110, 255));


	ImGui::PushStyleColor(ImGuiCol_ResizeGrip, IM_COL32(70, 70, 70, 100));
	ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, IM_COL32(120, 120, 120, 120));
	ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, IM_COL32(160, 160, 160, 200));


	ImGui::PushStyleColor(ImGuiCol_CheckMark, IM_COL32(170, 170, 170, 250));
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(120, 120, 120, 175));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, IM_COL32(160, 160, 160, 245));


	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

}


// =================================================


void nothing::UIStyleHelper::PushEvil()
{

	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(10, 0, 0, 250));
	ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(255, 0, 0, 255));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, IM_COL32(164, 0, 0, 255));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(255, 0, 0, 255));


	ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(250, 65, 65, 105));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(250, 40, 40, 185));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(250, 15, 15, 255));


	ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(125, 40, 40, 140));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(250, 65, 65, 105));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(255, 0, 0, 175));


	ImGui::PushStyleColor(ImGuiCol_ResizeGrip, IM_COL32(250, 65, 65, 70));
	ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, IM_COL32(255, 0, 0, 175));
	ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, IM_COL32(255, 0, 0, 240));


	ImGui::PushStyleColor(ImGuiCol_CheckMark, IM_COL32(250, 65, 65, 250));
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(255, 0, 0, 175));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, IM_COL32(255, 0, 0, 245));


	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.5f);

}


// =================================================


void nothing::UIStyleHelper::PopStyle()
{

	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(16);

}


// =================================================