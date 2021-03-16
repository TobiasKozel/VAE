#define TKLB_LEAKCHECKER_DISARM
#include "./TestCommon.h"
#include "../types/TFile.h"

int main() {
	{
		FileInfo file("./");
		file.scan();
	}
	memcheck()
	return 0;
}
