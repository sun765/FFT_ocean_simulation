#include "gui.h"

void myGUIStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.WindowRounding = 0.0f;
	style.FrameRounding = 0.0f;
	style.ScrollbarRounding = 0.0f;
	style.WindowPadding = ImVec2(20, 20);
	style.FramePadding = ImVec2(8, 5);
	style.ItemSpacing = ImVec2(10, 10);
	style.ItemInnerSpacing = ImVec2(10, 10);
	style.GrabMinSize = 20;
	style.TabRounding = 0.00f;

	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.65f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.298f, 0.603f, 0.164f, 0.65f);
	//style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(1.0f, 0.0f, 0.164f, 0.65f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.298f, 0.603f, 0.164f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.298f, 0.603f, 0.164f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.298f, 0.603f, 0.164f, 0.65f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.298f, 0.603f, 0.164f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.298f, 0.603f, 0.164f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.298f, 0.603f, 0.164f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.419f, 0.819f, 0.231f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.419f, 0.819f, 0.231f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.298f, 0.603f, 0.164f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.298f, 0.603f, 0.164f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.643f, 0.870f, 0.007f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.298f, 0.603f, 0.164f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.419f, 0.819f, 0.231f, 0.80f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.419f, 0.819f, 0.231f, 1.00f);
	//style.Colors[ImGuiCol_Header] = ImVec4(1.0f, 0.0f, 0.164f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.298f, 0.603f, 0.164f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.419f, 0.819f, 0.231f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.419f, 0.819f, 0.231f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.074f, 0.745f, 0.282f, 1.00f);
}



