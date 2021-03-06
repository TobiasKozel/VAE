<<includeIntrinsic>>

#include "../vae_faust_common.hpp"
#include "../../../wrapped/vae_profiler.hpp"
// We prevent namespace problems by including theses before the namsapces
// below. the includes inside from faust shouldn't cause any trouble then.
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

namespace vae { namespace core { namespace effect {
<<includeclass>>

struct Registermydsp {
	static EffectBase* create() {
		auto ptr = new mydsp();
		VAE_PROFILER_MALLOC_L(ptr, sizeof(mydsp), vae::core::profiler::dspAllocator)
		return ptr;
	}
	Registermydsp() {
		EffectsList::instance().add("mydsp", &create);
	}
} __registermydsp;

} } } // vae::core::effect
