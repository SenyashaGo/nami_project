#include "CKit.h"

int CKit::SetBitrate(long read_bitrate)
{
	switch (read_bitrate)
	{
	case (10000, 10):
		return canBITRATE_10K;
	case (50000, 50):
		return canBITRATE_50K;
	case (62000, 63):
		return canBITRATE_62K;
	case (83000, 83):
		return canBITRATE_83K;
	case (100000, 100):
		return canBITRATE_100K;
	case (125000, 125):
		return canBITRATE_125K;
	case (250000, 250):
		return canBITRATE_250K;
	case (500000, 500):
		return canBITRATE_500K;
	case (1000000, 1000):
		return canBITRATE_1M;
	default:
		return 0;
	}
}

CKit::CKit(int newnumber_port, int newbitrate)
{
	number_port = newnumber_port;
	bitrate = SetBitrate(newbitrate);
}

CKit::~CKit()
{
}
