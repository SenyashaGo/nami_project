#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include "canlib.h" 
#include "COutputChannel.h"
#include "CSettingsChannel.h"
#include "CGenerating_a_report_file.h"
#include "CShort_check.h"
#include "CKit.h"

class CChekChannels
{
public:
	CChekChannels(int newsend_number_port, std::vector <CKit> newkit, std::vector <CSettingsChannel> newsetap, std::vector <COutputChannel>* newresult);
	~CChekChannels();
	int SetBitrate(long read_bitrate);
	vector<canStatus> Start();
private:
	std::atomic<bool> stop = false;
	int send_number_port;
	std::vector <CKit> kit;
	std::vector <CSettingsChannel> setap;
	std::vector <COutputChannel> *result;
	bool verifyMSG(char* mass1, char* mass2);
	canStatus ConnectionToChannel(canHandle& handle, int channel_number, int bitrate);
	canStatus ReadMessageFromChnl(canHandle& handle, long& id, char* msg, unsigned int& dlc, unsigned int& flags, unsigned long& timestamp, int time);
	canStatus ThreatSend(std::atomic<bool>& stop, std::vector<CKit> kit, std::vector<CSettingsChannel>& setap, int send_number_port);
	canStatus ThreatRead(std::atomic<bool>& stop, std::vector<CKit> kit, std::vector<CSettingsChannel>&setap,
		int send_number_port, std::vector<COutputChannel>& result);
};

