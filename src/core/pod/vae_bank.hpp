#ifndef _VAE_BANK
#define _VAE_BANK

#include "../vae_types.hpp"
#include "./vae_source.hpp"
#include "./vae_event.hpp"
#include "./vae_mixer.hpp"

#include <string>
#include <vector>

namespace vae { namespace core {
	/**
	 * @brief Bank object containing Sources, Mixers and Events
	 * Can be loaded and unloaded at runtime
	 */
	struct Bank {
		std::string name;				// Name of the bank used for debugging
		std::string path;				// Path to the bank definition file
		std::vector<Source> sources;	// Audio sources defined
		std::vector<Mixer> mixers;		// Audio Mixers which can have effects ! is presorted !
		std::vector<Event> events;		// Events defined
		BankHandle id = InvalidBankHandle;
	};

	constexpr int _VAE_BANK_SIZE = sizeof(Bank);
} } // namespace vae::vore

#endif // _VAE_BANK
