#define TKLB_LEAKCHECKER_DISARM
#include "./TestCommon.hpp"
#include "../types/TFile.hpp"

int main() {
	{
		FileInfo file("./");
		file.scan();
	}
	memcheck()
	return 0;
}
