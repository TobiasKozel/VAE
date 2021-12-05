#ifndef _VAE_BANK_MANAGER
#define _VAE_BANK_MANAGER

#include "./vae_util.hpp"
#include "./vae_types.hpp"
#include "./pod/vae_bank.hpp"
#include "./fs/vae_bank_loader.hpp"

namespace vae { namespace core {
	/**
	 * @brief Holds all the banks
	 *
	 */
	class BankManager {
		using Mutex = tklb::SpinLock;
		using Lock = tklb::LockGuard<Mutex>;

		HeapBuffer<Bank> mBanks;		// All the currently loaded banks
		Mutex mMutex;					// Lock the bank for changes

	public:
		HeapBuffer<Bank>& all() {
			return mBanks;
		}

		void lock() {
			mMutex.lock();
		}

		void unlock() {
			mMutex.unlock();
		}

		/**
		 * @brief Iterate all loaded banks
		 *
		 * @tparam Func Callable type
		 * @param func	Callable
		 */
		template <class Func>
		void forEach(const Func&& func) {
			Lock l(mMutex);
			for (auto& i : mBanks) {
				if (i.id == InvalidBankHandle) { continue; }
				func(i);
			}
		}

		Bank& get(BankHandle bank) {
			VAE_ASSERT(mBanks[bank].id == bank)
			VAE_ASSERT(bank < mBanks.size())
			return mBanks[bank];
		}

		Result load(const char* path, const char* rootPath) {
			VAE_INFO("Loading bank from file %s%s", rootPath, path)
			Bank bank;
			auto result = BankLoader::load(path, rootPath, bank);
			if (result != Result::Success) {
				VAE_ERROR("Failed to load bank from file %s%s with error %i", rootPath, path, result)
				return result;
			}
			return load(bank);
		}

		Result load(Bank& bank) {
			// TODO init mixer effects

			if (bank.mixers.empty()) {
				// Ensure there's a master channel per bank
				bank.mixers.resize(1);
				bank.mixers[0].id = 0;
				bank.mixers[0].name = "Bank Master";
			}

			for (auto& m : bank.mixers) {
				m.buffer.resize(Config::MaxBlock, Config::MaxChannels);
			}

			{
				Lock l(mMutex);
				if (mBanks.size() < bank.id + 1) {
					mBanks.resize(bank.id + 1);
				}
				mBanks[bank.id] = std::move(bank);
			}
			VAE_INFO("Bank %s loaded.", mBanks[bank.id].name.c_str())
			return Result::Success;
		}

		Result addSource(BankHandle bankHandle, Source& source) {
			auto& bank = mBanks[bankHandle];
			Lock l(mMutex);
			if (bank.sources.size() <= source.id) {
				bank.sources.resize(source.id + 1);
			}
			bank.sources[source.id] = std::move(source);
			return Result::Success;
		}

		Result addEvent(BankHandle bankHandle, Event& event) {
			auto& bank = mBanks[bankHandle];
			Lock l(mMutex);
			if (bank.events.size() <= event.id) {
				bank.events.resize(event.id + 1);
			}
			bank.events[event.id] = std::move(event);
			return Result::Success;
		}

		Result addMixer(BankHandle bankHandle, Mixer& mixer) {
			// TODO init mixer effects
			mixer.buffer.resize(Config::MaxBlock, Config::MaxChannels);
			Lock l(mMutex);
			auto& bank = mBanks[bankHandle];
			if (bank.mixers.size() <= mixer.id) {
				bank.mixers.resize(mixer.id + 1);
			}
			bank.mixers[mixer.id] = std::move(mixer);
			return Result::Success;
		}

		Result unloadFromPath(const char* path) {
			Lock l(mMutex);
			for (auto& i : mBanks) {
				if (strcmp(path, i.path.c_str()) == 0) {
					return unloadFromId(i.id);
				}
			}
			VAE_WARN("Could not unload Bank %s", path)
			return Result::ElementNotFound;
		}

		Result unloadFromId(BankHandle bankHandle) {
			Lock l(mMutex);
			if (mBanks.size() <= bankHandle) {
				VAE_WARN("Could not unload bank with handle %i", bankHandle)
				return Result::ElementNotFound;
			}
			auto& bank = mBanks[bankHandle];
			VAE_ASSERT(bank.id == bankHandle)
			VAE_INFO("Unloading bank %s", bank.name.c_str())
			bank = { }; // should free all the memory
			return Result::Success;
		}

		void unloadAll() {
			for (auto& i : mBanks) {
				if (i.id == InvalidBankHandle) { continue; }
				unloadFromId(i.id);
			}
		}
	};
} } // namespace vae::core

#endif // _VAE_BANK_MANAGER
