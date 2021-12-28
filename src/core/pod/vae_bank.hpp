#ifndef _VAE_BANK
#define _VAE_BANK

#include "../vae_types.hpp"
#include "./vae_source.hpp"
#include "./vae_event.hpp"
#include "./vae_mixer.hpp"

namespace vae { namespace core {
	/**
	 * @brief Bank object containing Sources, Mixers and Events
	 * Can be loaded and unloaded at runtime
	 */
	struct Bank {
		HeapBuffer<Source> sources;		///< Audio sources defined
		HeapBuffer<Mixer> mixers;		///< Audio Mixers which can have effects ! is presorted !
		HeapBuffer<Event> events;		///< Events defined
		BankHandle id = InvalidBankHandle;
		NameString name;				///< Name of the bank used for debugging
		PathString path;				///< Path to the bank definition file
	};

	constexpr int _VAE_BANK_SIZE = sizeof(Bank);
} } // namespace vae::vore

#endif // _VAE_BANK
