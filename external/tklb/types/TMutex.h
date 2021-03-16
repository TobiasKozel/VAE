#ifndef TKLB_MUTEX
#define TKLB_MUTEX

#include <mutex>

#include "../util/TNoCopy.h"

namespace tklb {

class Mutex {
	std::mutex mMutex;
public:
	Mutex() {}

	void lock() {
		mMutex.lock();
	}

	void unlock() {
		mMutex.unlock();
	}

	TKLB_NO_COPY(Mutex)
};

class LockGuard {
	Mutex* mMutex = nullptr;
public:
	LockGuard(Mutex& mutex) {
		mMutex = &mutex;
		mutex.lock();
	}

	~LockGuard() {
		mMutex->unlock();
	}

	TKLB_NO_COPY(LockGuard)
};

} // namespace

#endif // TKLB_MUTEX
