#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include "canlib.h" 
#include "COutputChannel.h"
#include "CSettingsChannel.h"

class CChekChannels
{
public:
	CChekChannels(int newsend_number_port, std::vector <CSettingsChannel> newsetap, std::vector <COutputChannel>* newresult);
	~CChekChannels();
	int SetBitrate(long read_bitrate);
	void Start();
private:
	std::atomic<bool> stop = false;
	std::atomic<bool> pause = false;
	int send_number_port;
	std::vector <CSettingsChannel> setap;
	std::vector <COutputChannel> *result;
	bool verifyMSG(char* mass1, char* mass2);
	canStatus ConnectionToChannel(canHandle& handle, int channel_number, int bitrate);
	canStatus ThreatSend(std::atomic<bool>& stop, std::atomic<bool>& pause, std::vector<CSettingsChannel>& setap, int send_number_port);
	canStatus ReadMessageFromChnl(canHandle& handle, long& id, char* msg, unsigned int& dlc, 
		unsigned int& flags, unsigned long& timestamp);
	canStatus ThreatRead(std::atomic<bool>& stop, std::atomic<bool>& pause, std::vector<CSettingsChannel>& setap,
		int send_number_port, std::vector<COutputChannel>& result);
};

