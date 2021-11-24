#ifndef _VAE_EVENT
#define _VAE_EVENT

#include "../vae_types.hpp"

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
		std::string name;
		EventType type;
		std::vector<SourceHandle> sources;
		std::vector<EventHandle> on_start;
		std::vector<EventHandle> on_end;
	};
} } // namespace vae::vore

#endif // _VAE_EVENT
