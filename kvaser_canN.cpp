
#include <stdio.h>

#include "canlib.h"
#include <conio.h>
#include <thread>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;


void Check(const char* id, canStatus stat)
{
    if (stat != canOK) {
        char buf[50];
        buf[0] = '\0';
        canGetErrorText(stat, buf, sizeof(buf));
        printf("%s: failed, stat=%d (%s)\n", id, (int)stat, buf);
    }
}

void dumpMessageLoop(canHandle hnd, int channel_number) {

    canStatus stat = canOK;
    long id;
    unsigned int dlc, flags;
    unsigned char msg[8];
    DWORD timestamp;
    printf("Listening for messages on channel %d, press any key to close\n", channel_number);
    vector<int> mem;
    while (!_kbhit()) {

        stat = canReadWait(hnd, &id, msg, &dlc, &flags, &timestamp, 100);

        if (stat == canOK) {

            if (flags & canMSG_ERROR_FRAME) {
                printf("***ERROR FRAME RECEIVED***");
            }

            else if (id != 1792 and id != 1793) {
                printf("Id: %ld, Msg: %u %u %u %u %u %u %u %u length: %u Flags: %lu\n",
                    id, msg[0], msg[1], msg[2], msg[3], msg[4],
                    msg[5], msg[6], msg[7], dlc, timestamp);
            }

        }

        else if (stat != canERR_NOMSG) {
            Check("canRead", stat);
            break;
        }
    }
}

void send_msg(int ch_num, int bitrate, char id)
{
    canHandle hnd;

    canStatus stat;

    int channel_number = ch_num;

    char msg[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

    printf("Opening channel %d\n", channel_number);



    hnd = canOpenChannel(channel_number, canOPEN_EXCLUSIVE);

    if (hnd < 0) {

        Check("canOpenChannel", (canStatus)hnd);

        exit(1);
    }
    printf("Setting bitrate and going bus on\n");

    switch (bitrate)
    {
    case 500:
        stat = canSetBusParams(hnd, canBITRATE_500K, 0, 0, 0, 0, 0);
        break;
    case 250:
        stat = canSetBusParams(hnd, canBITRATE_250K, 0, 0, 0, 0, 0);
        break;
    case 10:
        stat = canSetBusParams(hnd, canBITRATE_10K, 0, 0, 0, 0, 0);
        break;
    case 100:
        stat = canSetBusParams(hnd, canBITRATE_100K, 0, 0, 0, 0, 0);
        break;
    case 125:
        stat = canSetBusParams(hnd, canBITRATE_125K, 0, 0, 0, 0, 0);
        break;
    case 1000:
        stat = canSetBusParams(hnd, canBITRATE_1M, 0, 0, 0, 0, 0);
        break;
    }

    Check("canSetBusParams", stat);

    stat = canBusOn(hnd);
    Check("canBusOn", stat);
    printf("Writing a message to the channel and waiting for it to be sent \n");
    
    char iddd = 0x600;
    stat = canWrite(hnd, id, msg, 8, 0);
    printf("send\n");

    Check("canWrite", stat);

    stat = canWriteSync(hnd, 100);
    Check("canWriteSync", stat);
    printf("Going off bus and closing channel");

    stat = canBusOff(hnd);
    Check("canBusOff", stat);

    stat = canClose(hnd);
    Check("canClose", stat);
}

void monitor_msg(int ch_num, int bitrate)
{

    canHandle hnd;

    canStatus stat;

    int channel_number = ch_num;

    
    printf("Opening channel %d\n", channel_number);

    hnd = canOpenChannel(channel_number, canOPEN_EXCLUSIVE);

    if (hnd < 0) {

        Check("canOpenChannel", (canStatus)hnd);

        exit(1);
    }
    printf("Setting bitrate and going bus on\n");

    switch (bitrate)
    {
    case 500:
        stat = canSetBusParams(hnd, canBITRATE_500K, 0, 0, 0, 0, 0);
        break;
    case 250:
        stat = canSetBusParams(hnd, canBITRATE_250K, 0, 0, 0, 0, 0);
        break;
    case 10:
        stat = canSetBusParams(hnd, canBITRATE_10K, 0, 0, 0, 0, 0);
        break;
    case 100:
        stat = canSetBusParams(hnd, canBITRATE_100K, 0, 0, 0, 0, 0);
        break;
    case 125:
        stat = canSetBusParams(hnd, canBITRATE_125K, 0, 0, 0, 0, 0);
        break;
    case 1000:
        stat = canSetBusParams(hnd, canBITRATE_1M, 0, 0, 0, 0, 0);
        break;
    }

    Check("canSetBusParams", stat);

    stat = canBusOn(hnd);
    Check("canBusOn", stat);

    dumpMessageLoop(hnd, channel_number);
    printf("Going of bus and closing channel");

    stat = canBusOff(hnd);
    Check("canBusOff", stat);

    stat = canClose(hnd);
    Check("canClose", stat);
}

void CheckForError(char cmd[50], canStatus stat);
int ListChannels();

int main(int argc, char* argv[]) 
{

    canInitializeLibrary();
    int nums_ch = ListChannels() - 2;
    printf("Found channels: %i\n", nums_ch);

    for (;;)
    {
        int ch_num_send, ch_num_mon, bitrate_send, bitrate_mon;
        char id_send;
        cout << "Enter channael number, bitrate, id(send): ";
        cin >> ch_num_send >> bitrate_send >> id_send;
        
        cout << endl;


        cout << "Enter channael number, bitrate(monitor): ";
        cin >> ch_num_mon >> bitrate_mon;
        cout << endl;

        


        thread fst(send_msg, ch_num_send, bitrate_send, 0);
        thread snd(monitor_msg, ch_num_mon, bitrate_mon);
        fst.join();
        snd.join();
    }




}



void CheckForError(char cmd[50], canStatus stat)
{
    //if stat not ok, print error
    if (stat != canOK)
    {
        char buf[255];
        buf[0] = '\0';
        canGetErrorText(stat, buf, sizeof(buf));
        printf("[%s] %s: failed, stat=%d\n", cmd, buf, (int)stat);
        printf("ABORT (Press any key)");
        getchar();
        abort();
    }
}

int ListChannels()
{
    canStatus stat;
    int number_of_channels = 0;
    int device_channel;
    char device_name[255];
    // Get number of channels
    stat = canGetNumberOfChannels(&number_of_channels);
    CheckForError((char*)"canGetNumberOfChannels", stat);
    if (number_of_channels > 0) {
        printf("%s %d %s\n", "Found", number_of_channels, "channels");
    }
    else {
        printf("Could not find any CAN interface.\n");
    }
    // Loop and print all channels
    for (int i = 0; i < number_of_channels; i++) {
        stat = canGetChannelData(i, canCHANNELDATA_DEVDESCR_ASCII, device_name, sizeof(device_name));
        CheckForError((char*)"canGetChannelData", stat);
        stat = canGetChannelData(i, canCHANNELDATA_CHAN_NO_ON_CARD, &device_channel, sizeof(device_channel));
        CheckForError((char*)"canGetChannelData", stat);
        printf("%s %d %s %d\n", "Found channel:", i, device_name, (device_channel + 1));
    }

    return number_of_channels;
}
