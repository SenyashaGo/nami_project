#include "CChekChannels.h"

CChekChannels::CChekChannels(int newsend_number_port, vector<CSettingsChannel> newsetap, vector<COutputChannel>& newresult)
{
	send_number_port = newsend_number_port;
	setap = newsetap;
	result = newresult;
}

CChekChannels::~CChekChannels()
{
}

bool CChekChannels::verifyMSG(char* mass1, char* mass2) {
	for (int i = 0;i < 8;++i)
		if (mass1[i] != mass2[i])
			return 0;
	return 1;
}

int CChekChannels::SetBitrate(long read_bitrate)
{
	switch (read_bitrate)
	{
	case (10000, 10):
		return canBITRATE_10K;
	case (50000, 50):
		return canBITRATE_50K;
	case (62000, 63):
		return canBITRATE_62K;
	case (83000, 83):
		return canBITRATE_83K;
	case (100000, 100):
		return canBITRATE_100K;
	case (125000, 125):
		return canBITRATE_125K;
	case (250000, 250):
		return canBITRATE_250K;
	case (500000, 500):
		return canBITRATE_500K;
	case (1000000, 1000):
		return canBITRATE_1M;
	default:
		return 0;
	}
}

canStatus CChekChannels::ConnectionToChannel(canHandle& handle, int channel_number, int bitrate)
{
	canHandle hnd;
	canStatus stat = canOK;

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

canStatus CChekChannels::ThreatSend(atomic<bool>& stop, atomic<bool>& pause, vector<CSettingsChannel>& setap, int send_number_port)
{
	canHandle hnd;
	canStatus stat;

	stat = ConnectionToChannel(hnd, send_number_port, setap[send_number_port].bitrate);
	if (stat != canOK)
		return stat;

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
			//this_thread::sleep_for(chrono::milliseconds(setap[send_number_port].periodicity));
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

canStatus CChekChannels::ReadMessageFromChnl(canHandle& handle, long& id, char* msg, unsigned int& dlc, unsigned int& flags, unsigned long& timestamp)
{
	canStatus stat;
	stat = canReadWait(handle, &id, msg, &dlc, &flags, &timestamp, 100);
	if (stat == canOK) {
		if (flags & canMSG_ERROR_FRAME)
			printf("***ERROR FRAME RECEIVED***");
	}
	return stat;
}

canStatus CChekChannels::ThreatRead(atomic<bool>& stop, atomic<bool>& pause, vector<CSettingsChannel>& setap, int send_number_port, vector<COutputChannel>& result) {
	canHandle hnd;
	canStatus stat=canOK;

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
		COutputChannel res = { number_port_read, false, false, false };
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
				res.cheak_id = true;
				timer = timestamp - sec;
				sec = timestamp;

				if (verifyMSG(msg, setap[send_number_port].msg))
					res.cheak_msg = true;
				if ((timer > setap[send_number_port].periodicity - 10) && (timer < setap[send_number_port].periodicity + 10))
					res.cheak_periodicity = true;
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

		result.push_back(res);
		printf("Going of bus and closing channel %i\n", number_port_read);
		stat = canBusOff(hnd);
		stat = canClose(hnd);
	}
	stop = true;
	return stat;
}

void CChekChannels::Start() {
	thread send([&]() {
		ThreatSend(stop, pause, setap, send_number_port);
		});


	thread read([&]() {
		ThreatRead(stop, pause, setap, send_number_port, result);
		});
}
