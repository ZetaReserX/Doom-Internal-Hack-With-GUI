#include "menu.hpp"

#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_impl_win32.h"
#include "../config.h"
#include "../dependencies/minhook/MinHook.h"
#include "../globals.h"
#include "../esp.h"

namespace ig = ImGui;

HWND window;

namespace Menu {
    void InitializeContext(HWND hwnd) {
        if (ig::GetCurrentContext( ))
            return;

        ImGui::CreateContext( );
        ImGui_ImplWin32_Init(hwnd);

        ImGuiIO& io = ImGui::GetIO( );
		window = hwnd;
        io.IniFilename = io.LogFilename = nullptr;
    }

	void UpdateManualInput(ImGuiIO& io) {
		POINT cursorPos;
		if (GetCursorPos(&cursorPos)) {
			ScreenToClient(window, &cursorPos);
			io.AddMousePosEvent((float)cursorPos.x, (float)cursorPos.y);
		}

		bool leftDown = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
		bool rightDown = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;

		static bool prevLeftDown = false;
		static bool prevRightDown = false;
		if (leftDown != prevLeftDown) {
			io.AddMouseButtonEvent(0, leftDown);
			prevLeftDown = leftDown;
		}
		if (rightDown != prevRightDown) {
			io.AddMouseButtonEvent(1, rightDown);
			prevRightDown = rightDown;
		}
	}

    void Render( ) {
		if (!bShowMenu && !config.ESPBool) {
			return;
		}
		else if (!bShowMenu && config.ESPBool) {
			RenderESP();
			return;
		}

		ImGuiIO& io = ImGui::GetIO();
		UpdateManualInput(io);

		if (config.AIMBool) {
			MH_EnableHook(updateAnglesAddress);
		}
		else if (!config.AIMBool) {
			MH_DisableHook(updateAnglesAddress);
		}

		ImGui::Begin("ImGui Window");
		ImGui::Checkbox("Vector AIM", &config.AIMBool);
		ImGui::Checkbox("ESP", &config.ESPBool);
		ImGui::Checkbox("ESP Health Bar", &config.ESPHealthBarBool);
		ImGui::End();
    }
} // namespace Menu
