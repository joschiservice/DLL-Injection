#include "includes.h"

//data
void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;

//hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {

	if (!pDevice)
		pDevice = o_pDevice;

	//

	oEndScene(pDevice);
}

DWORD WINAPI HackThread(HMODULE hModule) {
	//hook
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);
		MessageBox(NULL, "Creating Scene", "Injection", MB_OK);
		oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene,7);
	}
	else {
		MessageBox(NULL, "GetD3D9Device failed", "Injection", MB_OK);
	}

	while (!GetAsyncKeyState(VK_END)) {
		Sleep(1);
	}

	// uninject
	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpr) {
	if (reason == DLL_PROCESS_ATTACH)
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));

	return TRUE;
}