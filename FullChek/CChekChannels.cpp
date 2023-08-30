#include "CChekChannels.h"

using namespace std;

CChekChannels::CChekChannels(int newsend_number_port,vector<CKit> newkit, vector<CSettingsChannel> newsetap, vector<COutputChannel>* newresult)
{
	send_number_port = newsend_number_port;
	kit = newkit;
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

canStatus report(canStatus stat, atomic<bool>& stop) {
	stop = true;
	return stat;
}

canStatus CChekChannels::ConnectionToChannel(canHandle& handle, int channel_number, int bitrate)
{
	canHandle hnd;
	canStatus stat=canOK;

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
	return stat;
}

canStatus CChekChannels::ReadMessageFromChnl(canHandle& handle, long& id, char* msg, unsigned int& dlc, unsigned int& flags, unsigned long& timestamp, int time)
{
	canStatus stat = canOK;
	stat = canReadWait(handle, &id, msg, &dlc, &flags, &timestamp, time);
	if (stat == canOK) {
		if (flags & canMSG_ERROR_FRAME)
			printf("***ERROR FRAME RECEIVED***");
	}
	return stat;
}

canStatus CChekChannels::ThreatSend(atomic<bool>& stop, std::vector<CKit> kit, vector<CSettingsChannel>& setap, int send_number_port)
{
	canHandle hnd;
	canStatus stat=canOK;

	stat = ConnectionToChannel(hnd, setap[send_number_port].nubmer_port, kit[setap[send_number_port].nubmer_port].bitrate);
	if (stat != canOK)
		return report(stat, stop);

	chrono::time_point<chrono::steady_clock> time1, time2, time3;
	while (stop == false) {
		stat = canWrite(hnd, setap[send_number_port].id, setap[send_number_port].msg, 8, 0);
		if (stat != canOK)
			return report(stat, stop);
		printf("message is send\n");
		Sleep(setap[send_number_port].periodicity);
		stat = canWriteSync(hnd, 100);
		if (stat != canOK)
			return report(stat, stop);
	}

	printf("Going of bus and closing channel %i\n", setap[send_number_port].nubmer_port);
	stat = canBusOff(hnd);
	if (stat != canOK)
		return report(stat, stop);
	stat = canClose(hnd);
	if (stat != canOK)
		return report(stat, stop);



	stop = true;
	return stat;
}

int AvaregeTime(vector<int> per) {
	int sum = 0;
	if (per.size() < 3)
		return 0;
	for (int i = 0; i < per.size();i++) {
		if (i == 0 || i == 1)
			continue;
		sum += per[i];
	}
	return int(sum / (per.size() - 2));
}

canStatus CChekChannels::ThreatRead(atomic<bool>& stop, std::vector<CKit> kit, vector<CSettingsChannel>& setap, int send_number_port, vector<COutputChannel>& result) {
	canHandle hnd;
	canStatus stat=canOK;

	long id;
	unsigned int dlc, flags;
	char msg[8];
	DWORD timestamp;

	for (int number_port_read = 0; number_port_read < setap.size(); number_port_read++)
	{
		if (kit[number_port_read].number_port == setap[send_number_port].nubmer_port)
			continue;

		stat = ConnectionToChannel(hnd, kit[number_port_read].number_port, kit[number_port_read].bitrate);
		if (stat != canOK) {
			printf("error connection\n");
			continue;
		}
		printf("Going of bus and closing channel %i\n", kit[number_port_read].number_port);
		stat = canBusOff(hnd);
		if (stat != canOK)
			return report(stat, stop);
		stat = canClose(hnd);
		if (stat != canOK)
			return report(stat, stop);
		stat = ConnectionToChannel(hnd, kit[number_port_read].number_port, kit[number_port_read].bitrate);
		if (stat != canOK) {
			printf("error connection\n");
			continue;
		}

		int cnt = 0;
		char mass[8]{};
		vector<int> per;
		per.clear();
		COutputChannel res = { kit[number_port_read].number_port, false, false, false, -1, mass, -1 };
		chrono::time_point<chrono::steady_clock> time1, time2, time3, time4;

		int t = 0;
		printf("Listening messages on channel %d\n", kit[number_port_read].number_port);
		Sleep(500);
		time1 = chrono::high_resolution_clock::now();
		while (stop != true) {

			time2 = chrono::high_resolution_clock::now();
			std::chrono::duration<double> duration = time2 - time1;
			if (duration.count() > int(setap[send_number_port].periodicity*0.012))
				break;

			stat = ReadMessageFromChnl(hnd, id, msg, dlc, flags, timestamp, setap[send_number_port].periodicity);
			if (stat != canOK) {
				printf("cant read message\n");
				return report(stat, stop);
			}

			if (id == 0x700 || id == 0x701)
				continue;

			res.id = id;
			for (int i = 0;i < 8;i++)
				res.msg[i] = msg[i];

			if (id == setap[send_number_port].id) {
				printf("Id: %ld, Msg: %u %u %u %u %u %u %u %u length: %u Flags: %lu\n",
					id, msg[0], msg[1], msg[2], msg[3], msg[4],
					msg[5], msg[6], msg[7], dlc, timestamp);
				res.cheak_id = true;

				time3 = chrono::high_resolution_clock::now();
				std::chrono::duration<double> timer = time3 - time4;
				time4 = time3;
				t = int(timer.count() * 1000);
				per.push_back(t);
				printf("%i\n", t);

				res.cheak_msg = verifyMSG(msg, setap[send_number_port].msg);

				cnt += 1;
			}

			if (cnt >= 7) {
				res.periodicity = AvaregeTime(per);
				if (res.periodicity > int(setap[send_number_port].periodicity * 0.9) && res.periodicity < int(setap[send_number_port].periodicity * 1.1))
					res.cheak_periodicity = true;

				break;
			}
		}

		result.push_back(res);
		printf("Going of bus and closing channel %i\n", kit[number_port_read].number_port);
		stat = canBusOff(hnd);
		if (stat != canOK)
			return report(stat, stop);
		stat = canClose(hnd);
		if (stat != canOK)
			return report(stat, stop);
	}
	stop = true;
	return stat;
}

vector <canStatus> CChekChannels::Start() {
	vector<canStatus> stat;
	thread send([&]() {
		stat.push_back(ThreatSend(stop, kit, setap, send_number_port));
		});


	thread read([&]() {
		stat.push_back(ThreatRead(stop, kit, setap, send_number_port, *result));
		});

	send.join();
	read.join();

	return stat;
}
