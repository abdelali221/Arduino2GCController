#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <windows.h>
#include <dinput.h>

LPDIRECTINPUT8 g_lpDI = NULL;
LPDIRECTINPUTDEVICE8 g_lpDIDeviceController = NULL;

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) {
    HRESULT hr;

    hr = g_lpDI->CreateDevice(pdidInstance->guidInstance, &g_lpDIDevice, NULL);
    if (FAILED(hr)) {
        return DIENUM_STOP;
    }

    hr = g_lpDIDevice->SetDataFormat(&c_dfDIJoystick);
    if (FAILED(hr)) {
        g_lpDIDevice->Release();
        g_lpDIDevice = NULL;
        return DIENUM_STOP;
    }

    hr = g_lpDIDevice->SetCooperativeLevel(GetForegroundWindow(), DISCL_EXCLUSIVE | DISCL_FOREGROUND);
    if (FAILED(hr)) {
        g_lpDIDevice->Release();
        g_lpDIDevice = NULL;
        return DIENUM_STOP;
    }

     hr = g_lpDIDevice->Acquire();
    if (FAILED(hr)) {
        g_lpDIDevice->Release();
        g_lpDIDevice = NULL;
        return DIENUM_STOP;
    }

    return DIENUM_STOP; // Stop after finding the first joystick
}


int main(int argc, char *argv[])
{
    printf("\n====== Arduino2GCController ======");
    printf("\n===== Created by Abdelali221 =====");
    printf("\n\n          Setup Utility");
    printf("\n\n Checking available controllers...");
    DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&g_lpDI, NULL);
    g_lpDI->EnumDevices(DIClass_GameController, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);

    return 0;
}