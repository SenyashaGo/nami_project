#include "CSettingsChannel.h"

CSettingsChannel::CSettingsChannel(int newnubmer_port, long newid, char newmsg[8], unsigned long newperiodicity, bool newtypr_id) {
	nubmer_port = newnubmer_port;
	id = newid;
	for (int i = 0;i < 8;i++)
		msg[i] = newmsg[i];
	periodicity = newperiodicity;
	type_id = newtypr_id;
}

CSettingsChannel::~CSettingsChannel() {

}

