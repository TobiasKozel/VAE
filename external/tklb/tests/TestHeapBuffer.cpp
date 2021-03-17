#include "./TestCommon.hpp"
#include "../types/THeapBuffer.hpp"

int ObjectCount = 0;

class LifeCycleTest {
public:
	int id;

	LifeCycleTest() {
		id = ObjectCount;
		ObjectCount++;
	}

	~LifeCycleTest() {
		ObjectCount--;
	}
};



int main() {
	{
		HeapBuffer<LifeCycleTest> heap;
		int size1 = 0, count1 = 0, size2 = 0, count2 = 0, size3 = 0, count3 = 0;
		heap.reserve(4);
		size1 = heap.size();
		count1 = ObjectCount;
		heap.resize(0);
		size2 = heap.size();
		count2 = ObjectCount;
		heap.resize(2);
		size3 = heap.size();
		count3 = ObjectCount;
		int ads = 0;
	}

	memcheck()
	return 0;
}
