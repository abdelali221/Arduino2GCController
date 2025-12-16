#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <windows.h>
#include <dinput.h>

#define HANDSHAKE_SEND "A2GCC_HOST"
#define HANDSHAKE_RECEIVE "A2GCC_AVR"

typedef struct gc_controller
{
    uint8_t data1;
    uint8_t data2;
    uint8_t stickx;
    uint8_t sticky;
} gc_controller;

//LPDIRECTINPUT8 g_lpDI = NULL;
//LPDIRECTINPUTDEVICE8 g_lpDIDeviceController = NULL;

void PutParity(gc_controller *gc) {
    uint8_t onbits = 0;
    for (int i = 0; i < 8; i++) {
    onbits += (gc->data1 >> i) & 1; 
  }
  for (int i = 0; i < 4; i++) {
    onbits += (gc->data2 >> i) & 1; 
  }
    if (onbits % 2 == 0) {
        gc->data2 |= 16; 
    } else {
        if (gc->data2 & 16) gc->data2 -= 16; 
    }
}


int main(int argc, char *argv[])
{
    printf("\n====== Arduino2GCController ======");
    printf("\n===== Created by Abdelali221 =====");

    if(argc < 2) {
        printf("\n\nUsage : %s COMx\n", argv[0]);
        return -1;
    }

    printf("\n\n Opening Port %s...", argv[1]);

    HANDLE port =  CreateFile(argv[1], GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if(port == INVALID_HANDLE_VALUE) {
        printf("Failed! Error 0x%lx\n", GetLastError());
        return 0;
    }

    printf("Success!\n");

    FlushFileBuffers(port);

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
    if (!GetCommState(port, &dcbSerialParams)) {
        printf(" ERROR! Failed to get port state.\n");
        return -1;
    }
    dcbSerialParams.BaudRate=115200;
    dcbSerialParams.ByteSize=8;
    dcbSerialParams.StopBits=ONESTOPBIT;
    dcbSerialParams.Parity=NOPARITY;
    if(!SetCommState(port, &dcbSerialParams)){
        printf(" ERROR! Failed to set port state.\n");
        return -1;
    }

    COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout=500;
    timeouts.ReadTotalTimeoutConstant=50;
    timeouts.ReadTotalTimeoutMultiplier=100;
    timeouts.WriteTotalTimeoutConstant=500;
    timeouts.WriteTotalTimeoutMultiplier=100;

    if(!SetCommTimeouts(port, &timeouts)){
        printf(" ERROR! Failed to set timeouts.");
        return -1;
    }   

    printf(" Performing handshake with the Arduino...\n");

    DWORD bytesRead;

    int ret = WriteFile(port, HANDSHAKE_SEND, sizeof(HANDSHAKE_SEND), &bytesRead, NULL);
    if(!ret){
        printf(" ERROR! Failed to send data. ret = %d", ret);
        return -1;
    }

    char response[sizeof(HANDSHAKE_RECEIVE) + 1];

    bytesRead = 0;

    ret = ReadFile(port, response, sizeof(HANDSHAKE_RECEIVE) - 1, &bytesRead, NULL);
    if(!ret){
        printf(" ERROR! Failed to send data. ret = %d", ret);
        return -1;
    }

    if(!strcmp(response, HANDSHAKE_RECEIVE)) {
        printf(" Done.");
    } else {
        printf(" ERROR! COM device didn't answer to the handshake request.\n Make sure that you chose the right port and flashed the Firmware correctly.\n Received : %s", response);
        return -1;
    }

    gc_controller gc = {255, 0, 0, 0};

    PutParity(&gc);
    
    printf("\n Sending : %x, %x, %x, %x\n", gc.data1, gc.data2, gc.stickx, gc.sticky);
    
    //uint8_t start;

    
        PutParity(&gc);
        WriteFile(port, &gc, 4, &bytesRead, NULL);

        ReadFile(port, &gc, 4, &bytesRead, NULL);
        printf("\r Received : %x, %x, %x, %x", gc.data1, gc.data2, gc.stickx, gc.sticky);

    CloseHandle(port);
    return 0;
}