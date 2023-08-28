#pragma once
class COutputChannel
{
public:
	COutputChannel(int newnubmer_port, bool newcheak_id, bool newcheak_msg, bool newcheak_periodicity, long newid, char newmsg[8], int newperiodicity);
	~COutputChannel();
	int nubmer_port;
	long id;
	char msg[8];
	int periodicity;
	bool cheak_id;
	bool cheak_msg;
	bool cheak_periodicity;
};

