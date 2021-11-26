#ifndef _VAE_EVENT
#define _VAE_EVENT

#include "../vae_types.hpp"
#include "./vae_mixer.hpp"

#include <string>
#include <vector>

namespace vae { namespace core {
	struct Event {
		enum class EventType {
			undefined = 0,
			start,
			stop,
			emit
		};
		EventHandle id = InvalidHandle;
		EventType type;
		MixerHandle mixer = Mixer::MasterMixerHandle;
		std::string name;
		SourceHandle source;
		std::vector<EventHandle> on_start;
		std::vector<EventHandle> on_end;
	};
} } // namespace vae::vore

#endif // _VAE_EVENT
