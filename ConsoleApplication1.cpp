#include <stdio.h>
#include <vector>
#include <fstream>
#include <conio.h> 
#include "canlib.h" 
#include <thread> 
#include <map>
#include <string>
#include <atomic>
#include <Windows.h>
#include <iostream>
using namespace std;


struct one_channel {
	int nubmer_port;
	long id;
	int bitrate;
	char msg[8];
	unsigned long periodicity;
};

struct cheakf {
	int nubmer_port;
	bool cheak_id;
	bool cheak_msg;
	bool cheak_periodicity;
};

bool verifyMSG(char* mass1, char* mass2) {
	for (int i = 0;i < 8;++i)
		if (mass1[i] != mass2[i])
			return 0;
	return 1;
}

int FillKit(vector<one_channel>& kit) {
	ifstream in("data.txt");
	if (!in.is_open())
		return -1;

	int a;
	long b;
	char msg[8];
	double p;
	while (in >> a >> b) {
	}

	in.close();
	return 0;
}

string Check(const char* id, canStatus stat) {
	if (stat != canOK) {
		char buf[50];
		buf[0] = '\0';
		canGetErrorText(stat, buf, sizeof(buf));
		return ("%s: failed, stat=%d (%s)\n", id, (int)stat, buf);
	}
	return("OK");
}

bool SetBitrate(int& bitrate, long read_bitrate) {
	switch (read_bitrate)
	{
	case 10000:
		bitrate = canBITRATE_10K;
		return 1;
	case 50000:
		bitrate = canBITRATE_50K;
		return 1;
	case 62000:
		bitrate = canBITRATE_62K;
		return 1;
	case 83000:
		bitrate = canBITRATE_83K;
		return 1;
	case 100000:
		bitrate = canBITRATE_100K;
		return 1;
	case 125000:
		bitrate = canBITRATE_125K;
		return 1;
	case 250000:
		bitrate = canBITRATE_250K;
		return 1;
	case 500000:
		bitrate = canBITRATE_500K;
		return 1;
	case 1000000:
		bitrate = canBITRATE_1M;
		return 1;
	default:
		return 0;
	}
}

canStatus ConnectionToChannel(canHandle& handle, int channel_number, int bitrate) {

	canHandle hnd;
	canStatus stat;

	hnd = canOpenChannel(channel_number, canOPEN_EXCLUSIVE);

	if (hnd < 0) {
		return (canStatus)hnd;
	}

	stat = canSetBusParams(hnd, bitrate, 0, 0, 0, 0, 0);
	if (stat != canOK)
		return stat;


	stat = canBusOn(hnd);
	if (stat != canOK)
		return stat;

	handle = hnd;
	printf("channel %i is open\n", channel_number);
	return canOK;
}

canStatus ThreatSend(atomic<bool>& stop, atomic<bool>& pause, vector<one_channel>& setap, int send_number_port) {
	canHandle hnd;
	canStatus stat;

	stat = ConnectionToChannel(hnd, send_number_port, setap[send_number_port].bitrate);
	if (stat != canOK)
		return stat;

	chrono::time_point<chrono::steady_clock> time1, time2, time3;
	while (stop == false) {
		while (pause == false && stop == false) {
			stat = canWrite(hnd, setap[send_number_port].id, setap[send_number_port].msg, 8, 0);
			if (stat != canOK)
				return 	stat;
			printf("message is send\n");
			stat = canWriteSync(hnd, setap[send_number_port].periodicity);
			if (stat != canOK)
				return stat;
			Sleep(setap[send_number_port].periodicity);
			//printf("message is send\n");
		}
		Sleep(100);
	}

	printf("Going of bus and closing channel %i\n", send_number_port);
	stat = canBusOff(hnd);
	if (stat != canOK)
		return stat;
	stat = canClose(hnd);
	if (stat != canOK)
		return stat;

	return canOK;
}

canStatus ReadMessageFromChnl(canHandle& handle, long& id, char* msg, unsigned int& dlc, unsigned int& flags, unsigned long& timestamp) {
	canStatus stat;
	stat = canReadWait(handle, &id, msg, &dlc, &flags, &timestamp,100);
	if (stat == canOK) {
		if (flags & canMSG_ERROR_FRAME)
			printf("***ERROR FRAME RECEIVED***");
	}
	return stat;
}

canStatus ThreatRead(atomic<bool>& stop, atomic<bool>& pause, vector<one_channel>& setap, int send_number_port, vector<cheakf>& cheak) {
	canHandle hnd;
	canStatus stat;

	long id;
	unsigned int dlc, flags;
	char msg[8];
	DWORD timestamp;

	for (int number_port_read = 0;number_port_read < setap.size();number_port_read++)
	{
		if (number_port_read == send_number_port)
			continue;

		stat = ConnectionToChannel(hnd, number_port_read, setap[number_port_read].bitrate);
		if (stat != canOK) {
			printf("error connection\n");
			continue;
		}
		cheakf ch = { number_port_read, false, false, false };
		int sec = 0;
		int timer = 0;
		int cnt = 0;
		chrono::time_point<chrono::steady_clock> time1, time2;

		printf("Listening messages on channel %d\n", number_port_read);
		time1 = chrono::high_resolution_clock::now();
		while (true) {
			Sleep(50);
			stat = ReadMessageFromChnl(hnd, id, msg, dlc, flags, timestamp);
			if (stat != canOK) {
				return stat;
				printf("cant read message\n");
				break;
			}

			if (id == setap[send_number_port].id) {
				printf("Id: %ld, Msg: %u %u %u %u %u %u %u %u length: %u Flags: %lu\n",
					id, msg[0], msg[1], msg[2], msg[3], msg[4],
					msg[5], msg[6], msg[7], dlc, timestamp);
				ch.cheak_id = true;
				timer = timestamp - sec;
				sec = timestamp;

				if (verifyMSG(msg, setap[send_number_port].msg))
					ch.cheak_msg = true;
				if (timer > setap[send_number_port].periodicity - 10 && timer < setap[send_number_port].periodicity + 10)
					ch.cheak_periodicity = true;
				cnt += 1;
			}
			else printf("error id %li\n", id);
			if (cnt >= 2)
				break;
			time2 = chrono::high_resolution_clock::now();
			std::chrono::duration<double> duration = time2 - time1;
			if (duration.count() > 10)
				break;
		}

		cheak.push_back(ch);
		printf("Going of bus and closing channel %i\n", number_port_read);
		stat = canBusOff(hnd);
		Check("canBusOff", stat);
		stat = canClose(hnd);
		Check("canClose", stat);
	}
	stop = true;
	return stat;
}






int main() {
	canInitializeLibrary();

	vector<one_channel> setap;


	atomic<bool> pause = false;
	atomic<bool> stop = false;

	int bitrate;
	char msg[8] = { 1, 2, 3, 4, 5, 6, 7, 8};
	SetBitrate(bitrate, 500000);
	struct one_channel a1 = { 0, 0x300, bitrate, msg[0], msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7], 500 };
	struct one_channel a2 = { 1, 0x600, bitrate, msg[0], msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7], 500 };
	struct one_channel a3 = { 2, 0x400, bitrate, msg[0], msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7], 500 };

	setap.push_back(a1);
	setap.push_back(a2);
	setap.push_back(a3);

	for (int i = 0;i < setap.size(); i++) {
		printf("%i %li %i", setap[i].nubmer_port, setap[i].id, setap[i].bitrate);
		for (int j = 0;j < 8; j++)
			printf("%c ", setap[i].msg[j]);
		printf("%li\n", setap[i].periodicity);
	}

	vector<cheakf> cheaking;




	int send_number_port = 0;

	thread send([&]() {
		ThreatSend(stop, pause, setap, send_number_port);
		});


	thread read([&]() {
		ThreatRead(stop, pause, setap, send_number_port, cheaking);
		});





	send.join();
	read.join();

	printf("\noutput result\n");
	printf("n_port  id  msg  per\n");		
	for (int i = 0;i < cheaking.size(); i++) {
		printf("%4i %4i %4i %4i\n", cheaking[i].nubmer_port, cheaking[i].cheak_id, cheaking[i].cheak_msg, cheaking[i].cheak_periodicity);
	}
}





































































//void Check(const char* id, canStatus stat) {
//    if (stat != canOK) {
//        char buf[50];
//        buf[0] = '\0';
//        canGetErrorText(stat, buf, sizeof(buf));
//        printf("%s: failed, stat=%d (%s)\n", id, (int)stat, buf);
//        exit(1);
//    }
//}
//bool dumpMessageLoop(canHandle hnd, int channel_number, long idh, char mass[]) {
//    canStatus stat = canOK;
//    long id;
//    unsigned int dlc, flags;
//    unsigned char msg[8];
//    DWORD timestamp;
//    printf("Listening for messages on channel %d, press any key to close\n", channel_number);
//    while (!_kbhit()) {
//
//        stat = canReadWait(hnd, &id, msg, &dlc, &flags, &timestamp, 100);
//        if (stat == canOK) {
//            if (flags & canMSG_ERROR_FRAME) {
//                printf("***ERROR FRAME RECEIVED***");
//            }
//            else {
//                printf("Id: %ld, Msg: %u %u %u %u %u %u %u %u length: %u Flags: %lu\n",
//                    id, msg[0], msg[1], msg[2], msg[3], msg[4],
//                    msg[5], msg[6], msg[7], dlc, timestamp);
//
//
//
//                if (id == idh) {
//                    int cnt = 0;
//                    for (int i = 0;i < dlc;i++)
//                        if (mass[i] == msg[i])
//                            cnt += 1;
//
//                    if (cnt == dlc) {
//                        printf("i read correct massage from channel %i\n", channel_number);
//                        return true;
//                    }
//                    else
//                    {
//                        printf("i read uncorrect massage from channel %i\n", channel_number);
//                        return true;
//                    }
//                }
//            }
//        }
//        else if (stat != canERR_NOMSG) {
//            Check("canRead", stat);
//            break;
//        }
//    }
//}
//
//
//
//int main(int argc, int* argv[]) {
//
//    canInitializeLibrary();
//
//    int CountChannels = 3;
//    int ch = 0;
//    char msg[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
//    long idh = 0x100;
//
//
//
//    thread read([&]()
//        {
//            canHandle hnd1;
//            canStatus stat1;
//            for (int i = 1; i < CountChannels; i++)
//            {
//                printf("Opening channel %d\n", i);
//                hnd1 = canOpenChannel(i, canOPEN_EXCLUSIVE);
//                if (hnd1 < 0) {
//                    Check("canOpenChannel", (canStatus)hnd1);
//                    exit(1);
//                }
//
//                printf("Setting bitrate and going bus on\n");
//
//                stat1 = canSetBusParams(hnd1, canBITRATE_500K, 0, 0, 0, 0, 0);
//                Check("canSetBusParams", stat1);
//
//                stat1 = canBusOn(hnd1);
//                Check("canBusOn", stat1);
//
//                if (dumpMessageLoop(hnd1, i, idh, msg) == true)
//                    ch += 1;
//
//                printf("Going of bus and closing channel\n");
//
//                stat1 = canBusOff(hnd1);
//                Check("canBusOff", stat1);
//
//                stat1 = canClose(hnd1);
//                Check("canClose", stat1);
//            }
//        });
//
//
//
//    thread send([&]()
//        {
//            canHandle hnd;
//            canStatus stat;
//            int channel_number = 0;
//            printf("Opening channel %d\n", channel_number);
//            hnd = canOpenChannel(channel_number, canOPEN_EXCLUSIVE);
//            if (hnd < 0) {
//                Check("canOpenChannel", (canStatus)hnd);
//                exit(1);
//            }
//            printf("Setting bitrate and going bus on\n");
//            stat = canSetBusParams(hnd, canBITRATE_500K, 0, 0, 0, 0, 0);
//            Check("canSetBusParams", stat);
//            stat = canBusOn(hnd);
//            Check("canBusOn", stat);
//            printf("Writing a message to the channel and waiting for it to be sent \n");
//            while (!_kbhit()) {
//                stat = canWrite(hnd, idh, msg, 8, 0);
//                Check("canWrite", stat);
//                Sleep(100);
//            }
//
//            stat = canWriteSync(hnd, 100);
//            Check("canWriteSync", stat);
//            printf("Going off bus and closing channel");
//            stat = canBusOff(hnd);
//            Check("canBusOff", stat);
//            stat = canClose(hnd);
//            Check("canClose", stat);
//            if (ch == CountChannels - 1)
//            {
//                exit(1);
//            }
//        });
//
//
//
//
//
//
//    send.join();
//    read.join();
//
//}
