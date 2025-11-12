#include <Windows.h>
#include <iostream>
#include <thread>

#include "console/console.hpp"

#include "hooks/hooks.hpp"
#include "utils/utils.hpp"

#include "dependencies/minhook/MinHook.h"

#include "offsets.h"
#include "vectorAim.h"

angles* CurrentAngles;

typedef void(__fastcall* updateAngles_t)(long long param_1, angles* param_2, float* param_3);
updateAngles_t originalUpdateAngles = nullptr;
void __fastcall updateAngles(long long param_1, angles* param_2, float* param_3);

DWORD WINAPI OnProcessAttach(LPVOID lpParam);
DWORD WINAPI OnProcessDetach(LPVOID lpParam);

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);

        U::SetRenderingBackend(VULKAN);

        HANDLE hHandle = CreateThread(NULL, 0, OnProcessAttach, hinstDLL, 0, NULL);
        if (hHandle != NULL) {
            CloseHandle(hHandle);
        }
    }
    else if (fdwReason == DLL_PROCESS_DETACH && !lpReserved) {
        OnProcessDetach(NULL);
    }

    return TRUE;
}

void InitializeDetours() {
    if (!originalUpdateAngles) {
        LOG("[!] originalUpdateAngles == nullptr, адрес не инициализирован");
        return;
    }

    updateAnglesAddress = reinterpret_cast<LPVOID>(originalUpdateAngles);

    MH_STATUS st = MH_CreateHook(
        updateAnglesAddress,
        reinterpret_cast<LPVOID>(&updateAngles),
        reinterpret_cast<LPVOID*>(&originalUpdateAngles)
    );

    if (st != MH_OK) {
        LOG("[!] MH_CreateHook(UpdateAngles) failed: %d", static_cast<int>(st));
        return;
    }

    LOG("[+] Hooked UpdateAngles. updateAnglesAddress=%p trampoline=%p", updateAnglesAddress, originalUpdateAngles);
}

void InitializeAddresses() {
    originalUpdateAngles = (updateAngles_t)(mainModule + 0xE5D470);
    InitializeDetours();
}

DWORD WINAPI OnProcessAttach(LPVOID lpParam) {
    Console::Alloc();
    LOG("[+] Rendering backend: %s\n", U::RenderingBackendToStr());
    if (U::GetRenderingBackend() == NONE) {
        LOG("[!] Looks like you forgot to set a backend. Will unload after pressing enter...");
        std::cin.get();

        FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(lpParam), 0);
        return 0;
    }

    MH_Initialize();
    H::Init();
    InitializeAddresses();

    return 0;
}

DWORD WINAPI OnProcessDetach(LPVOID lpParam) {
    MH_RemoveHook(updateAnglesAddress);
    H::Free();
    MH_Uninitialize();

    Console::Free();

    return 0;
}

void __fastcall updateAngles(long long param_1, angles* param_2, float* param_3)
{
    CurrentAngles = param_2;
    VectorAim();

    originalUpdateAngles(param_1, CurrentAngles, param_3);
}