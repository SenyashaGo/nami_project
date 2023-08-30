#pragma once
#include <canlib.h>
class CKit
{
public:
	int SetBitrate(long read_bitrate);
	CKit(int newnumber_port, int newbitrate);
	~CKit();
	int number_port;
	int bitrate;
private:

};

