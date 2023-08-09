
#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include "canlib.h"
using namespace std;
void Check(const char* id, canStatus stat) {
    if (stat != canOK) {
        char buf[50];
        buf[0] = '\0';
        canGetErrorText(stat, buf, sizeof(buf));
        printf("%s: потерпел неудачу, статус=%d (%s)\n", id, (int)stat, buf);
        exit(1);
    }
}
void dumpMessageLoop(canHandle hnd, int channel_number/*, int channel_number1*/)
{
    canStatus stat = canOK;
    /*canHandle hnd;*/
    //canHandle hnd1;
    long id = 0x100;
    unsigned int dlc, flags;
    char msg[8];
    /*hnd = canOpenChannel(channel_number, canOPEN_EXCLUSIVE);
    if (hnd < 0)
    {
        Check("canOpenChannel", (canStatus)hnd);
        exit(1);
    }
    printf("Установка скорости передачи данных и включение шины\n");
    stat = canSetBusParams(hnd, canBITRATE_250K, 0, 0, 0, 0, 0);
    Check("canSetBusParams", stat);
    stat = canBusOn(hnd);
    Check("canBusOn", stat);*/
    /*hnd1 = canOpenChannel(channel_number1, canOPEN_ACCEPT_VIRTUAL);
    if (hnd1 < 0)
    {
        Check("canOpenChannel", (canStatus)hnd1);
        exit(1);
    }
    stat = canSetBusParams(hnd1, canBITRATE_250K, 0, 0, 0, 0, 0);
    Check("canSetBusParams", stat);
    stat = canBusOn(hnd1);
    Check("canBusOn", stat);*/
    DWORD timestamp;
    printf("Прослушивание сообщений на канале %d, нажмите любую клавишу, чтобы закрыть\n", channel_number);
    while (!_kbhit()) {
  
        stat = canReadWait(hnd, &id, msg, &dlc, &flags, &timestamp, 3000);
      
        if (stat == canOK) 
        {
            if (flags & canMSG_ERROR_FRAME) 
            {
                printf("***ERROR FRAME RECEIVED***");
            }
            else 
            {
                printf("Id: %ld, Msg: %u %u %u %u %u %u %u %u length: %u Flags: %lu\n",
                    id, msg[0], msg[1], msg[2], msg[3], msg[4],
                    msg[5], msg[6], msg[7], dlc, timestamp);
                /*stat = canBusOff(hnd);
                Check("canBusOff", stat);
                stat = canClose(hnd);
                Check("canClose", stat);*/
            }
        }
        else if (stat != canERR_NOMSG) 
        {
            Check("canRead", stat);
            /*stat = canBusOff(hnd);
            Check("canBusOff", stat);
            stat = canClose(hnd);
            Check("canClose", stat);*/
            break;
        }
    }
}
int main() 
{
    setlocale(0, "");
    canHandle hnd;
    canHandle hnd1;
    canStatus stat;
    char msg[8] = { 1,2,3,4,5,6,7,8 };
    int channel_number = 0;
    int channel_number1 = channel_number + 1;
    canInitializeLibrary();
    printf("Открывающий канал %d\n", channel_number);
    hnd = canOpenChannel(channel_number, canOPEN_ACCEPT_VIRTUAL);
    if (hnd < 0) 
    {
        Check("canOpenChannel", (canStatus)hnd);
        exit(1);
    }
    printf("Установка скорости передачи данных и включение шины\n");
    stat = canSetBusParams(hnd, canBITRATE_500K, 0, 0, 0, 0, 0);
    Check("canSetBusParams", stat);
    stat = canBusOn(hnd);
    Check("canBusOn", stat);
    thread th(dumpMessageLoop, hnd, channel_number/*, channel_number1*/);
    hnd1 = canOpenChannel(channel_number1, canOPEN_ACCEPT_VIRTUAL);
    if (hnd1 < 0)
    {
        Check("canOpenChannel", (canStatus)hnd1);
        exit(1);
    }
    stat = canSetBusParams(hnd1, canBITRATE_500K, 0, 0, 0, 0, 0);
    Check("canSetBusParams", stat);
    stat = canBusOn(hnd1);
    Check("canBusOn", stat);
    for (int i = 0; i < 100; i++)
    {
        stat = canWrite(hnd1, 0x100, msg, 8, 0);
        Check("canWrite", stat);
    }
    printf("Отправление автобуса и закрытие канала\n");
    stat = canBusOff(hnd);
    Check("canBusOff", stat);
    stat = canClose(hnd);
    Check("canClose", stat);
    stat = canBusOff(hnd1);
    Check("canBusOff", stat);
    stat = canClose(hnd1);
    Check("canClose", stat);
    th.join();
    system("pause");
}
