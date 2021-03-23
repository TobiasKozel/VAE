#ifndef TKLBZ_MUTEX
#define TKLBZ_MUTEX

#include <mutex>

namespace tklb {
	class Mutex {
		std::mutex mMutex;
	public:
		Mutex(const Mutex&) = delete;
		Mutex(const Mutex*) = delete;
		Mutex(Mutex&&) = delete;
		Mutex& operator= (const Mutex&) = delete;
		Mutex& operator= (Mutex&&) = delete;

		Mutex() = default;

		void lock() {
			mMutex.lock();
		}

		void unlock() {
			mMutex.unlock();
		}
	};

	class LockGuard {
		Mutex& mMutex;
	public:
		LockGuard(const LockGuard&) = delete;
		LockGuard(const LockGuard*) = delete;
		LockGuard(LockGuard&&) = delete;
		LockGuard& operator= (const LockGuard&) = delete;
		LockGuard& operator= (LockGuard&&) = delete;

		LockGuard(Mutex& mutex) : mMutex(mutex) {
			mutex.lock();
		}

		~LockGuard() {
			mMutex.unlock();
		}
	};
} // namespace

#endif // TKLB_MUTEX
