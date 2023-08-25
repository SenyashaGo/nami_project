#include "COutputChannel.h"

COutputChannel::COutputChannel(int newnubmer_port, bool newcheak_id, bool newcheak_msg, bool newcheak_periodicity, long newid, char newmsg[8], int newperiodicity)
{
	nubmer_port = newnubmer_port;
	cheak_id = newcheak_id;
	cheak_msg = newcheak_msg;
	cheak_periodicity = newcheak_periodicity;
	id = newid;
	for (int i = 0;i < 8;i++)
		msg[i] = newmsg[i];
	periodicity = newperiodicity;
}

COutputChannel::~COutputChannel()
{
}
