#ifndef _VAE_HRTF
#define _VAE_HRTF

#include "../vae_types.hpp"
#include "../vae_config.hpp"

namespace vae { namespace core {
	struct HRTF {
		struct Position {
			Vec3 pos;
			AudioBuffer ir;
		};
		HeapBuffer<Position> positions;
		Size originalRate = 0;
		Size rate = 0;
	};
} } // namespace vae::vore

#endif // _VAE_HRTF
