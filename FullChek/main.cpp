#include <iostream>
#include "CChekChannels.h"

int main() {
	using namespace std;
	canInitializeLibrary();
	vector<CSettingsChannel> setap;
	char msg[8] = { '01', '02', '03', '04', '05', '06', '07', '08'};
	CSettingsChannel a1(0, 0x300, 500, msg, 100),
		a2(1, 0x600, 500, msg, 100), 
		a3(0, 0x400, 500, msg, 100);

	setap.push_back(a1);
	setap.push_back(a2);
	setap.push_back(a3);
	vector<COutputChannel> result;
	int send_number_port = 0;
	CChekChannels a(send_number_port, setap, &result);
	a.Start();

	printf("\noutput result\n");
	printf("n_port  id  msg  per\n");
	for (int i = 0;i < result.size(); i++) {
		printf("%4i %4i %4i %4i %6li %s %6i\n",
			result[i].nubmer_port, result[i].cheak_id, result[i].cheak_msg, result[i].cheak_periodicity, result[i].id, result[i].msg, result[i].periodicity);
	}
	return 0;
}