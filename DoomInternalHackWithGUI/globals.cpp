#include "globals.h"

int clientWidth = 1280;
int clientHeight = 720;
HMODULE hModule = GetModuleHandleA("DOOMx64vk.exe");
uintptr_t mainModule = (uintptr_t)hModule;
LPVOID updateAnglesAddress;