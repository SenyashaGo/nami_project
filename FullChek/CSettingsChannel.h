#pragma once
#include "canlib.h" 
class CSettingsChannel
{
public:
	CSettingsChannel(int newnubmer_port, long newid, char newmsg[8], unsigned long newperiodicity, bool newtype_id);
	~CSettingsChannel();
	int nubmer_port;
	long id;
	char msg[8];
	unsigned long periodicity;
	bool type_id;
};

