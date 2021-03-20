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
		int size = 0, count = 0;
		heap.reserve(4);
		size = heap.size();
		count = ObjectCount;
		heap.resize(0);
		size = heap.size();
		count = ObjectCount;
		heap.resize(2);
		int id = heap[1].id;
		size = heap.size();
		count = ObjectCount;
	}
	memcheck()
	{
		HeapBuffer<int> heap;
		int size = 0, count = 0;
		heap.reserve(4);
		size = heap.size();
		count = ObjectCount;
		heap.resize(0);
		size = heap.size();
		count = ObjectCount;
		heap.resize(2);
		size = heap.size();
		count = ObjectCount;
	}

	memcheck()
	{
		HeapBuffer<LifeCycleTest*> heap;
		int size = 0, count = 0;
		heap.reserve(4);
		size = heap.size();
		count = ObjectCount;
		heap.resize(0);
		size = heap.size();
		count = ObjectCount;
		heap.resize(2);
		size = heap.size();
		count = ObjectCount;
	}


	return 0;
}
