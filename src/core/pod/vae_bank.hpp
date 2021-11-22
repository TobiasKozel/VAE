#ifndef _VAE_BANK
#define _VAE_BANK

#include <string>
#include <vector>

#include "../vae_types.hpp"
#include "./vae_source.hpp"
#include "./vae_event.hpp"

namespace vae { namespace core {
	struct Bank {
		BankHandle id;					// Not used
		std::string name;				// Name of the bank
		std::string path;				// Path to the bank definition file
		std::vector<Source> sources;	// Audio sources defined
		std::vector<Event> events;		// Events defined
	};
} } // namespace vae::vore

#endif // _VAE_BANK
