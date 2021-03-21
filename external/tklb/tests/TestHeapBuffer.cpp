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
		heap.reserve(4);
		if (heap.size() != 0) {
			return 1;
		}
		heap.resize(0);
		if (heap.size() != 0) {
			return 2;
		}
		heap.resize(2);
		if (heap.size() != 2) {
			return 3;
		}
		if (heap[0].id != 0) {
			return 4;
		}
		if (heap[1].id != 1) {
			return 5;
		}
		if (ObjectCount != 2) {
			return 6;
		}
	}
	memcheck()
	{
		HeapBuffer<int> heap;
		heap.reserve(4);
		heap.resize(0);
		heap.resize(2);
	}

	memcheck()
	{
		HeapBuffer<LifeCycleTest*> heap;
		heap.reserve(4);
		heap.resize(0);
		heap.resize(2);
	}
	return 0;
}
