#pragma once
#include "canlib.h" 
class CSettingsChannel
{
public:
	CSettingsChannel(int newnubmer_port, long newid, int newbitrate, char newmsg[8], unsigned long newperiodicity);
	~CSettingsChannel();
	int SetBitrate(long read_bitrate);
	int nubmer_port;
	long id;
	int bitrate;
	char msg[8];
	unsigned long periodicity;
	bool type_id;
};

