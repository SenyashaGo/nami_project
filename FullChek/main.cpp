#include <iostream>
#include "CChekChannels.h"

int main() {
	using namespace std;
	setlocale(LC_ALL, "RUS");
	canInitializeLibrary();
	vector <CKit> kit;
	CKit k(0, 500), k1(1, 500), k2(2, 500);
	kit.push_back(k1);
	kit.push_back(k);
	kit.push_back(k2);

	vector<CSettingsChannel> setap;
	char msg[8] = { 00,01,02,03,04,05,06,07 };
	CSettingsChannel a1(0, 0x300, msg, 300, 1),
		a2(0, 0x306, msg, 500, 1),
		a3(2, 0x400, msg, 500, 1);

	setap.push_back(a1);
	setap.push_back(a2);
	setap.push_back(a3);
	vector<COutputChannel> result;
	int send_number = 0;
	CChekChannels a(send_number, kit, setap, &result);
	vector<canStatus> stat;
	stat=a.Start();

	for (int i = 0; i < stat.size(); ++i)
		printf("error code - %i\n\n", int(stat[i]));

	printf("output result:\n");
	printf("n_port  id  msg  per\n");
	for (int i = 0;i < result.size(); i++) {
		printf("%4i %4i %4i %4i %6li %8s %6i\n",
			result[i].nubmer_port, result[i].cheak_id, result[i].cheak_msg, result[i].cheak_periodicity, result[i].id, result[i].msg, result[i].periodicity);
	}


	ÑShort_check f(result, setap);
	CGenerating_a_report_file u(result, setap, "data");
	

	return 0;
}

