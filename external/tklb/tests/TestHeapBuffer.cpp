#include "./TestCommon.hpp"
#include "../types/THeapBuffer.hpp"

int ObjectCount = 0;
int ConstructorCount = 0;

class LifeCycleTest {
public:
	int id;
	int payload = 0;
	LifeCycleTest() {
		id = ObjectCount;
		ObjectCount++;
		ConstructorCount++;
	}

	~LifeCycleTest() {
		ObjectCount--;
	}
};

int main() {
	{
		HeapBuffer<LifeCycleTest> heap;

		{ // Test resize reserve
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


		{ // Test push pop
			LifeCycleTest push;
			push.payload = 1337;
			heap.push(push);

			LifeCycleTest pop;
			heap.pop(&pop);
			if (pop.payload != 1337) {
				return 7;
			}
		}

		if (ObjectCount != 2) {
			return 8;
		}

		{
			LifeCycleTest* push = TKLB_NEW(LifeCycleTest);
			LifeCycleTest* pop = TKLB_NEW(LifeCycleTest);
			heap.push(*push);
			heap.pop(pop);
			TKLB_DELETE(push);
			TKLB_DELETE(pop);
		}
	}

	if (ObjectCount != 0) {
		return 8;
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
	memcheck()

	{
		{
		HeapBuffer<LifeCycleTest*> heap;
		heap.reserve(4);
		heap.resize(0);
		heap.resize(2);
	}
	}
	return 0;
}
