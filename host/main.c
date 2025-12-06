#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <windows.h>

#define HANDSHAKE_SEND "A2GCC_HOST"
#define HANDSHAKE_RECEIVE "A2GCC_AVR" 

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
        printf("Failed! Error 0x%x\n", GetLastError());
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
    dcbSerialParams.BaudRate=9600;
    dcbSerialParams.ByteSize=8;
    dcbSerialParams.StopBits=ONESTOPBIT;
    dcbSerialParams.Parity=NOPARITY;
    if(!SetCommState(port, &dcbSerialParams)){
        printf(" ERROR! Failed to set port state.\n");
        return -1;
    }

    COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout=50000;
    timeouts.ReadTotalTimeoutConstant=50;
    timeouts.ReadTotalTimeoutMultiplier=100;
    timeouts.WriteTotalTimeoutConstant=50;
    timeouts.WriteTotalTimeoutMultiplier=10;

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

    ret = ReadFile(port, response, sizeof(HANDSHAKE_RECEIVE), &bytesRead, NULL);
    if(!ret){
        printf(" ERROR! Failed to send data. ret = %d", ret);
        return -1;
    }

    if(!strcmp(response, HANDSHAKE_RECEIVE)) {
        printf(" Done.");
    } else {
        printf(" ERROR! COM device didn't answer to the handshake request.\n Make sure that you chose the right port and flashed the Firmware correctly.");

    }
    
    CloseHandle(port);
    return 0;
}