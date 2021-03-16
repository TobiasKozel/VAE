#include "TestCommon.h"
struct Test {
	int a;
	int b;
};

int main() {
	Test* test = new Test();
	delete test;
	auto test2 = new Test[10];
	delete[] test2;
	void* test3 = malloc(10);
	test3 = realloc(test3, 12);
	free(test3);

	memcheck()
	return 0;
}
