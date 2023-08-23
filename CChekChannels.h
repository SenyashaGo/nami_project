#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include "canlib.h" 
#include "COutputChannel.h"
#include "CSettingsChannel.h"

using namespace std;

class CChekChannels
{
public:
	CChekChannels(int newsend_number_port, vector <CSettingsChannel> newsetap, vector <COutputChannel>& newresult);
	~CChekChannels();
	int SetBitrate(long read_bitrate);
	void Start();
private:
	atomic<bool> stop = false;
	atomic<bool> pause = false;
	int send_number_port;
	vector <CSettingsChannel> setap;
	vector <COutputChannel> result;
	bool verifyMSG(char* mass1, char* mass2);
	canStatus ConnectionToChannel(canHandle& handle, int channel_number, int bitrate);
	canStatus ThreatSend(atomic<bool>& stop, atomic<bool>& pause, vector<CSettingsChannel>& setap, int send_number_port);
	canStatus ReadMessageFromChnl(canHandle& handle, long& id, char* msg, unsigned int& dlc, 
		unsigned int& flags, unsigned long& timestamp);
	canStatus ThreatRead(atomic<bool>& stop, atomic<bool>& pause, vector<CSettingsChannel>& setap, 
		int send_number_port, vector<COutputChannel>& result);
};

